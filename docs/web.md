# Web publishing

The collected tilde.club configuration uses nginx, PHP-FPM, and fcgiwrap.
Apache's `mod_userdir`, `.htaccess`, and `a2enmod` do not configure this stack.

## Production layout

The main site's document root is `/usr/share/nginx/html`. Its server block
serves HTTP and HTTPS. Several separate service vhosts redirect HTTP to HTTPS,
but the main host's block serves both. The main nginx config includes
`/etc/nginx/sites-enabled/*`; this is a local layout to reproduce explicitly,
not an assumption about the distribution default.

| Request | Backend or directory |
| --- | --- |
| Main site | `/usr/share/nginx/html` |
| `/~USER/` and `/u/USER/` | `/home/USER/public_html/` |
| User PHP | `/run/php-fpm/php-fpm-php-user.sock`, pool UID `php-user`, group `club` |
| Main site PHP | `/run/php-fpm/www.sock`, pool UID/group `nginx` |
| User CGI | `/run/fcgiwrap/fcgiwrap-cgi.sock`; service runs as `cgiwrap` |
| `/~USER/git` | Local cgit wrapper through fcgiwrap |
| `/~USER/myplant` | Central Botany PHP application |
| WeeChat relay beneath a user page | A socket beneath the user's `.weechat` directory |
| Webmail | SnappyMail in a separate vhost |

The user-page location enables directory listings and SSI, and uses a per-user
HTML 404 page. Account for those features when replacing a running config.

The collected pool generator can create per-user FPM sockets, but the main
user-page route uses the **shared** pool. `makeuser` does not call the pool
generator. Do not describe this as per-user PHP execution.

The CGI unit has UID/GID-changing capabilities. Those capabilities do not, by
themselves, establish that each script is switched to its owner. That needs the
wrapper implementation and a runtime identity test. Likewise, the collected
CGI `SCRIPT_FILENAME` expression prepends a home path to `$request_filename`
inside an alias location. Verify the expanded path before using it in a new
config. The new-host examples below use explicit per-account paths instead.

## Start with static pages

Use [tilde-static.conf](../examples/nginx/tilde-static.conf) inside nginx's
`http` context and replace the example domain. It provides both user URL forms,
blocks dotfiles and script-source downloads, and disables symlinks in public
user content. The example leaves directory listings and SSI off; enable those
features deliberately if your community wants them.

A 0711 home can be traversed without being listed. Public directories need
0755 and public files 0644. Keep the user's other files private. Check every
path component with `namei -l`, not just the page's final mode.

For a new SELinux-enforcing host, use the installed policy's home publishing
support. A common Fedora layout is:

```bash
setsebool -P httpd_enable_homedirs on
semanage fcontext -a -t httpd_user_content_t '/home/[^/]+/public_html(/.*)?'
restorecon -RFv /home/member/public_html
```

If a matching custom rule already exists, review it and use `semanage fcontext
-m` to modify it instead of adding a duplicate. Test access as the service and
inspect AVC denials. Policy versions and service domains can require additional
booleans or labels for writing or connecting to other services; enable only the
access actually needed. The production snapshot with SELinux disabled cannot
validate those policy rules for a new enforcing installation.

Useful checks:

```bash
nginx -t
namei -l /home/member/public_html/index.html
ls -Zd /home/member /home/member/public_html
ausearch -m AVC,USER_AVC -ts recent
journalctl -u nginx --since today
```

The production host sends nginx logs to syslog; do not assume every nginx error
will be in `/var/log/nginx/error.log`. Check the effective `error_log` setting.

## Add HTTPS

After obtaining a certificate, retain the HTTP challenge location and decide
whether other HTTP requests should be served or redirected. For an HTTPS-only
policy, redirect the non-challenge requests to a fixed configured hostname,
rather than constructing a redirect from an unvalidated Host header.

Copy the publishing locations into an HTTPS server block with these settings,
using the certificate paths returned by Certbot:

```nginx
listen 443 ssl;
listen [::]:443 ssl;
http2 on;
server_name tilde.example.net;
ssl_certificate /etc/letsencrypt/live/tilde.example.net/fullchain.pem;
ssl_certificate_key /etc/letsencrypt/live/tilde.example.net/privkey.pem;
ssl_protocols TLSv1.2 TLSv1.3;
```

Keep shared locations in one root-owned include if both server blocks use them.
Set response headers in the context where they are intended to apply. Test
headers on error responses and nested locations; inheritance is version-specific.
A blanket CSP, frame policy, or cross-origin isolation policy on every member's
page can break the projects you are hosting. Keep the operator application and
member publishing policies separate. Add HSTS only after deciding that all of
its affected names will continue to support HTTPS.

A Certbot deploy hook for nginx can be a root-owned executable file in
`/etc/letsencrypt/renewal-hooks/deploy/`:

```bash
#!/usr/bin/env bash
set -euo pipefail
/usr/sbin/nginx -t
/usr/bin/systemctl reload nginx
```

Enable the packaged renewal timer and test renewal. The collected production
service instead invokes `/usr/local/bin/certbot renew --standalone`, with hooks
to stop and start nginx. Its timer checks at 06:09 and 18:09 in the host's local
time, with `Persistent=true`. That produces a different availability profile
from webroot renewal. Verify all certificate consumers after renewal, including
Dovecot and Postfix if they share the certificate.

## Add PHP with a deliberate execution identity

A new host can give an enabled member a dedicated pool. Install maintained
packages, including the process extension if you use the identity test below:

```bash
dnf install php-fpm php-cli php-process
```

Review [member.conf](../examples/php-fpm/member.conf), then install it as
`/etc/php-fpm.d/member.conf`. The Unix account and group must already exist.
Create the private session directory named in that file:

```bash
sudo -u member -- install -d -m 0700 /home/member/.local/state/php/sessions
php-fpm -t
systemctl enable --now php-fpm
```

If PHP-FPM is already running, reload it after successful validation. Ensure the
nginx worker can connect to the new socket. With SELinux enabled, directory
labels, FPM's domain, and socket policy need to be checked on the target system.

Insert [member-php.inc](../examples/nginx/member-php.inc) into the site's server
block **after** the dotfile-denial location but **before** the generic script
source-denial and static userdir locations. Keep both HTTP and HTTPS policy
consistent. The example supports `.php` endpoints for this named member; it
does not implement PATH_INFO routing for arbitrary frameworks. If you want PHP
index files, add `index.php` to the userdir index list after enabling its handler.

Validate and reload nginx. Use this temporary test in the member's public
directory, then remove it:

```php
<?php

declare(strict_types=1);

header('Content-Type: text/plain; charset=utf-8');
echo 'sapi=', PHP_SAPI, "\n";
echo 'uid=', posix_geteuid(), "\n";
```

Compare the response UID with `id -u member`. Do not use `get_current_user()` as
an execution-identity test: it reports the script file's owner. Also test that
the backend cannot read a second member's private file. FPM running as the member
still has all the access that member has; it is not a container or a separate
security identity from their shell.

Tune `pm.max_children`, process lifetime, memory, uploads, and logging to the
machine. Avoid eagerly starting several workers for every dormant account.
The collected shared pool uses 20 maximum children and a 120-second request
limit; those are production settings for that pool, not universal defaults.

## CGI

A new installation can use Fedora's `fcgiwrap@.service` template with a separate
instance for an enabled member. Inspect the installed unit with `systemctl cat`
and confirm the service runs as that member. Override the matching socket's
owner/group so nginx can connect, keeping the socket at 0660. Do not grant a
shared CGI worker UID-changing capabilities merely to imitate the current unit.

Restrict a new CGI handler to the specific member's `public_html/cgi-bin` path.
Use a fixed `SCRIPT_FILENAME` mapping for that account, verify the target is a
real executable, and test the interpreter, UID, environment, and filesystem
access before enabling it. Keep source-file denial for scripts without an
enabled handler. PATH_INFO and executable index support need their own tests.
The existing host's generalized CGI wrapper was not supplied, so this guide
does not claim it can reproduce its owner-switching behavior.

## Publishing tests

Use two disposable members and check response bodies and filesystem effects,
not just status codes:

- Both user URL forms, nested directories, index files, and trailing-slash redirects.
- Missing users and scripts, dotfiles, percent-encoded paths, and traversal attempts.
- A symlink from public content to a nonpublic readable file.
- PHP source denial before enabling PHP, then the correct runtime UID afterward.
- A script under one account trying to read or write the other account's private data.
- User custom error pages, directory listings, SSI, and optional routes where enabled.
- TLS certificate validation and all response headers on successful and error responses.

Remove diagnostics and downloadable configuration archives from the web root
after use. Keep configs root-owned, validate before reload, and keep the previous
config available for rollback.
