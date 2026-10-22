# Configuration examples

These are starting configurations for a **new** host. They are not a dump of
tilde.club's live configs and are not installed by applying the repository patch.
Replace example domains and accounts, read the linked chapter, and validate on
the target host before reloading a service.

| File | Context | Instructions |
| --- | --- | --- |
| `ssh/00-tilde.conf` | Global sshd config/drop-in | [SSH](../docs/ssh.md) |
| `nginx/tilde-static.conf` | nginx `http` context | [Web](../docs/web.md) |
| `nginx/member-php.inc` | nginx server block; ordering matters | [PHP](../docs/web.md#add-php-with-a-deliberate-execution-identity) |
| `php-fpm/member.conf` | PHP-FPM pool | [PHP](../docs/web.md#add-php-with-a-deliberate-execution-identity) |
| `dovecot/tilde.conf` | Standalone Dovecot 2.4 config | [Mail](../docs/mail.md) |
| `postfix/main.cf.fragment` | Merge into Postfix `main.cf` | [Mail](../docs/mail.md) |
| `postfix/master.cf.fragment` | Merge named submission services | [Mail](../docs/mail.md) |
| `gemserv/config.toml` | Version-matched Gemini configuration | [Publishing](../docs/publishing.md) |
| `gemserv/gemserv.service` | systemd service for the new-host layout | [Publishing](../docs/publishing.md) |
| `borgmatic/config.yaml` | borgmatic 2.x with Borg 1.x | [Backups](../docs/backups.md) |

The mail fragments deliberately do not overwrite distribution service entries,
install credentials, or open firewall ports. The backup example needs a remote
repository, protected passphrase file, and prepared database dumps.
