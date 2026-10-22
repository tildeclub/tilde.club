# Build a tilde-style machine

A tilde is a shared Unix machine where people get a shell account and space to
publish things. The machine needs an operator, a backup, and some clear rules.
It does not need every service on the existing tilde.club host on the first day.

This guide uses Fedora Server and nginx. The production reference was collected
from tilde.club on October 22, 2026, running Fedora 43. Use a supported release
and its maintained packages. See [the system record](current-system.md) for
what was actually observed; examples below use `tilde.example.net` and a test
account named `member`.

The commands in this guide are Bash. If your interactive shell is Fish, start
`bash` before using examples that assign variables or use Bash loops. Run
administrative commands from a root shell obtained with `sudo -i`, unless the
text says to run them on your own computer or as a regular user.

## 1. Plan the machine

For a small initial group, 2 CPUs, 4 GiB RAM, and 60 GiB of SSD storage are a
reasonable starting allocation. Compilers, persistent bots, mail, and games can
change that quickly. Monitor actual use before increasing limits or buying more
capacity. The existing server's size is not a minimum requirement.

You need:

- A domain you control and a host with a stable public address.
- Provider console or recovery access independent of SSH.
- An off-host backup destination and a place to keep recovery keys.
- An admin account, a second person or recovery process for emergencies, and a
  published way to contact the operators.
- A decision about signups, acceptable use, account removal, and disk limits.

Use separate service accounts for the web server, mail, Gopher, and other
services. Ordinary shell accounts do not belong in `wheel` or a container-engine
administration group. Shell access lets people run programs: PHP restrictions
and a blocked SSH forwarding option do not turn a shell account into a sandbox.

## 2. Install and update Fedora

Install Fedora Server with SSH access and time synchronization enabled. Keep
SELinux enforcing on a new installation. The production host currently has it
disabled; that is not a prerequisite for running a tilde.

```bash
dnf upgrade --refresh
dnf install sudo openssh-server git curl rsync tmux vim-enhanced nano jq \
    acl ripgrep policycoreutils-python-utils firewalld chrony
hostnamectl set-hostname tilde.example.net
systemctl enable --now sshd chronyd
```

Reboot if the update requires it and verify a fresh login. Record the OS version
and package sources. Do not add random repositories to recover an old program
name from a 2014 package list.

## 3. Set DNS and the firewall

Create an A record for `tilde.example.net`. Add an AAAA record only if IPv6 works
through the host and provider firewalls. Test both address families from outside
the server. Reverse DNS is especially important if you plan to send mail directly.

On a new host using firewalld, first check which zone owns the network interface:

```bash
firewall-cmd --get-active-zones
```

In the example below, replace `public` with that zone. Confirm any custom SSH
port is allowed before changing a firewall remotely.

```bash
systemctl enable firewalld
firewall-cmd --permanent --zone=public --add-service=ssh
firewall-cmd --permanent --zone=public --add-service=http
firewall-cmd --permanent --zone=public --add-service=https
firewall-cmd --reload
firewall-cmd --zone=public --list-all
```

These commands assume firewalld is already running. If it is stopped, prepare
the persistent zone with `firewall-offline-cmd` before starting it, including
your actual SSH port and the intended network-zone assignment. Then start it
and verify a new connection. Provider firewalls are independent.

The existing tilde.club host uses CSF. Do not enable a second firewall manager
on that machine as part of following this guide. See [ports and firewall
policy](shellserver.md#ports-and-firewall-policy) before adding optional services.

## 4. Establish an admin account

Create a private primary group and home. Set a password for sudo, then add the
account to `wheel`:

```bash
useradd --create-home --user-group --shell /bin/bash operator
passwd operator
usermod --append --groups wheel operator
install -d -o operator -g operator -m 0700 /home/operator/.ssh
```

Copy the admin's **public** key to `/home/operator/.ssh/authorized_keys`, without
replacing an existing key file. Set its owner to `operator:operator` and mode
0600, and run `restorecon -RF /home/operator/.ssh` if SELinux is enabled.

From a second terminal on your own computer, verify key login and `sudo -v`.
Keep the first connection open. Do not disable the only working login method
before its replacement has been tested.

Follow [SSH access and MFA](ssh.md) to make the key-only baseline explicit.
That chapter also covers enrollment before enforcing MFA and a separate legacy
SSH service. Do not copy the production host's account-specific exceptions.

## 5. Build the home skeleton

Follow [the skeleton instructions](etc-skel-permissions.md). At a minimum, create
an empty private SSH directory, a public HTML directory, and a useful starter
page. Do this before creating the test account.

The public [tilde.etcskel repository](https://github.com/tildeclub/tilde.etcskel)
is a reference to review, not a directory to copy over your live `/etc/skel`
without checking it. It contains host-specific defaults and first-login hooks.

## 6. Install the web service

```bash
dnf install nginx
install -d -o root -g root -m 0755 /usr/share/nginx/html
```

Use [web publishing](web.md) and the
[static nginx example](../examples/nginx/tilde-static.conf). Start with static
pages. That makes it possible to test path mapping and private-file access
before adding PHP or CGI.

The example belongs in `/etc/nginx/conf.d/tilde.conf`, which Fedora's normal
nginx configuration includes inside `http`. Check that include on your machine.
Remove or adjust a conflicting packaged default server block; do not keep two
servers both claiming to be the default on the same address and port.

Replace the example domain, then validate and start the service:

```bash
nginx -t
systemctl enable --now nginx
```

The example is HTTP-only so it can be used before obtaining a certificate. Set
up TLS before accepting account passwords or enabling web applications.

## 7. Create one test member

The following is the independent Unix-account baseline. It does not announce an
account, send welcome mail, create ZNC accounts, or update the production signup
queue. Those are integrations described in [accounts](accounts.md).

```bash
useradd --create-home --user-group --shell /bin/bash member
passwd member
chmod 0711 /home/member
install -d -o member -g member -m 0700 /home/member/.ssh
install -d -o member -g member -m 0755 /home/member/public_html
```

`useradd` should copy the empty key file from the prepared skeleton. Put the
member's public key there, then set its owner to `member:member` and mode 0600.
Never put private keys in the skeleton or generate a private key for a member
and email it to them.

Keep the home at 0711 for this layout. That permits traversal to public content
without allowing a directory listing of the home. Private directories and files
still need their own modes: use 0700 for private directories and 0600 for private
files. A 0711 home alone does not hide a readable file at a known path.

With SELinux enabled, apply and test the publishing policy described in the web
chapter, then restore the account's labels. Do not recursively make a home
world-readable just to fix a 403 response.

From your own computer, test SSH, an unrelated key that should fail, and:

```bash
curl --fail http://tilde.example.net/~member/
curl --fail http://tilde.example.net/u/member/
```

Create a second disposable member and run the cross-account checks in the web
chapter before enabling executable user content.

## 8. Add TLS

The static example exposes ACME challenges beneath `/var/lib/letsencrypt`.
Create that directory and install the packaged Certbot client:

```bash
dnf install certbot
install -d -o root -g root -m 0755 /var/lib/letsencrypt/.well-known/acme-challenge
certbot certonly --webroot -w /var/lib/letsencrypt -d tilde.example.net
```

Verify that the returned certificate paths exist. Add the HTTPS server block
from the web chapter, run `nginx -t`, and reload nginx. Install a deploy hook to
validate and reload nginx when a renewed certificate is deployed. If the same
certificate is used by mail, reload those consumers as well. Confirm the
packaged renewal timer is enabled and perform `certbot renew --dry-run`.

This webroot setup can renew without stopping nginx. The existing server uses
a separate standalone renewal service that stops and starts nginx; its details
are recorded in the web chapter rather than imposed on a new host.

## 9. Give people useful tools

Install a small set first; expand it in response to actual requests:

```bash
dnf install fish zsh emacs irssi weechat alpine lynx links \
    git gcc make python3 ruby perl figlet
```

Use `dnf info NAME` before adding optional packages. Packages, module streams,
and third-party builds change. Keep a record of manual installations and their
update procedure. Do not chmod system compilers to root-only as a substitute
for resource management; programming is part of the point of the machine.

Choose the shells you support in `/etc/shells`, and make sure each has working
startup files. Add [first-login setup](accounts.md#first-login-wizard) only after
its dependencies and exit behavior have been tested with those shells.

## 10. Set quotas and resource limits

Use [the shell operations chapter](shellserver.md). For XFS, discover the mount
containing `/home` before running quota commands. On the collected machine that
mount is `/`, not `/home`.

A disk quota does not limit CPU or processes. Set and verify those policies
separately, with enough room for normal compiles, editors, mail, and chat clients.

## 11. Back up and restore before opening signups

Follow [backup and recovery](backups.md). Include account data, mail, service
configs, local programs, web applications, and consistent database dumps. Keep
the repository and its recovery keys off the host. Restore a test account into
an isolated directory and verify the result.

A successful backup job alone is not a completed restore test.

## 12. Add the services you intend to operate

- [Mail](mail.md): local delivery, IMAP, submission, DNS, filtering, and abuse checks.
- [IRC](ircserver.md): clients, persistent sessions, ident, ZNC, and optional network hosting.
- [Gopher and Gemini](publishing.md): separate public directories and service users.
- [Usenet](usenet.md): local groups before opening reader access or negotiating peers.
- [Website and community tools](community.md): signup handling, the wiki, and optional apps.

Each service adds an update job, a backup requirement, and a support obligation.
Write down who owns it and how to stop or restore it.

## Before accepting members

- Test admin and member login from outside the host, including IPv6 if published.
- Verify MFA enrollment, account recovery, and any allowed automation accounts.
- Test static publishing, private-file denial, and executable content separately.
- Confirm disk quotas and process/memory limits with the test accounts.
- Complete an off-host backup and isolated restore.
- Publish rules, a contact address, maintenance notices, and an account-removal policy.
- Review every enabled network listener against the intended firewall policy.
- Remove diagnostic PHP pages, temporary archives, and test credentials from web roots.

Keep your local configuration changes under version control without secrets.
Record the version or commit used for every manually installed service. The
[upstream references](sources.md) explain the software-specific behavior behind
these examples.
