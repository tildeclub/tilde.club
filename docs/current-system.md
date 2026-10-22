# Current system

This records the operator's inventories and selected configs collected on
October 22, 2026. It describes the existing shell host. The
[installation guide](how-to-set-up-a-tilde.md) and [examples](../examples/README.md)
make separate choices for a new host; they do not silently claim those choices
are already deployed here.

## Machine

- Fedora Linux 43, x86-64, 8 logical CPUs, about 29 GiB usable RAM.
- About 1 TiB on the root XFS filesystem; `/home` and `/var` are on that mount.
- Root mount options include user and group quotas.
- SELinux disabled on the recorded host; CSF is the active firewall manager.

These are the resources on the existing machine, not minimum requirements.
Exact user inventories, addresses, private keys, credentials, and raw config
dumps are deliberately not part of this public record.

## Services

| Purpose | Confirmed configuration |
| --- | --- |
| Shell access | OpenSSH main service; separate restricted legacy service on 2222. |
| Authentication | Key login, or PAM password/TOTP path; the collected main config does not require both paths together. |
| Onboarding | Local `makeuser`, Ruby `firstlogin`, skeleton marker, Google Authenticator wrapper. |
| Web | nginx, root `/usr/share/nginx/html`, `/~USER/` and `/u/USER/` publishing. |
| PHP | Shared `php-user:club` pool for user pages; `nginx:nginx` pool for main site applications. |
| CGI | fcgiwrap instance with a local override running as `cgiwrap`. |
| Mail | Postfix 3.10.14, Dovecot/Pigeonhole 2.4.5, Maildir at `~/.mail`. |
| Mail filtering | OpenDKIM and Rspamd on configured local milter endpoints; pyspf-milter also running. |
| Webmail | nginx vhost points to SnappyMail. |
| Usenet | INN 2.7.3 running on the shell host. |
| Gopher | Fossor socket activation; dedicated `gopher` service identity. |
| Gemini | gemserv, running from `/srv/gemini/gemserv`, as `gemini`. |
| Finger | Local Python `fingerd.py`, service identity `efingerd`. |
| IRC support | oidentd and ZNC running. Network-server location/daemon was not established by this collection. |
| Database/cache | MariaDB and Valkey running. |
| Accounting | psacct active. |
| Certificates | Custom twice-daily standalone Certbot timer that stops/starts nginx. |
| Backups | Root cron at 03:00, explicit `/usr/bin/borgmatic` 2.1.9 with Borg 1.4.5. |

The compiled services and private application integration code need their own
version/update records. Installed RPMs are not an installation manifest: the
host has accumulated optional tools and old builds over time.

## Details worth keeping straight

`/usr/local/bin/borgmatic` is a different, older installation (1.5.21). Manual
invocation through PATH is not necessarily the program run by the backup job.

`/home` is not an independent mount here. Quota commands need the containing
mountpoint `/`; the collector's `/home` quota query printed an error.

The per-user FPM pool generator exists, but the collected main nginx route uses
the shared socket. A generated per-user pool alone does not route a member's
requests to it.

The supplied main SSH config does not include `sshd_config.d`; collecting those
files does not establish that the daemon reads them. The effective global
`AuthenticationMethods` was `any`. The SSH chapter explains the key and PAM
paths, with an explicit MFA example for a new installation.

The aggregate `user.slice` reported no finite memory or task limit. Individual
user slices and PAM limits were not comprehensively collected, so no complete
production resource policy is claimed here.

The Gopher service reads options from `/etc/default/fossor` and
`/etc/sysconfig/fossor`; those files were not supplied. Gemini's service uses
`/srv/gemini/gemserv/config.toml`, which was also not supplied. The publishing
chapter identifies the runtime layout and points to version-matched upstream
configuration rather than inventing live option values.

The backup repository, complete source list, retention, and restore history
were not supplied. The backup chapter's example is explicitly separate from the
confirmed scheduler and executable versions.
