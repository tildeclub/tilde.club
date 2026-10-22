# tilde.club

This repo documents how to build and run a shared Unix machine like
[tilde.club](https://tilde.club). It also keeps the older site utilities at their
existing paths.

Members get a shell account, space for their own pages, and tools for writing,
programming, chatting, and making whatever interests them. You do not need to
know all of Unix before joining. Helping people learn is part of running the
place.

## Build your own

Start with [Build a tilde-style machine](docs/how-to-set-up-a-tilde.md).
It follows a Fedora/nginx setup from a new host through account creation,
publishing, limits, backups, and launch checks.

The [current system record](docs/current-system.md) describes what was confirmed
on tilde.club on October 22, 2026. The [example configs](examples/README.md) are
separate starting points for a new host. Applying this repo patch does not
install them or change a running server.

| Topic | Guide |
| --- | --- |
| Base installation | [Build a tilde-style machine](docs/how-to-set-up-a-tilde.md) |
| Accounts and onboarding | [Account creation and first login](docs/accounts.md) |
| Login and recovery | [SSH keys and MFA](docs/ssh.md) |
| Home defaults | [Skeleton and permissions](docs/etc-skel-permissions.md) |
| Publishing | [nginx, PHP-FPM, and CGI](docs/web.md) |
| Mail | [Postfix, Dovecot 2.4, submission, and filtering](docs/mail.md) |
| Other publishing | [Gopher, Gemini, and finger](docs/publishing.md) |
| Chat | [IRC operations](docs/ircserver.md) and [connection details](docs/ircserver-connections.md) |
| News | [INN and local newsgroups](docs/usenet.md) |
| Daily operation | [Quotas, resource limits, firewall, and maintenance](docs/shellserver.md) |
| Recovery | [Borg/borgmatic backups and restore](docs/backups.md) |
| Usage records | [Login and process accounting](docs/accounting.md) |
| Website and apps | [Sources, signups, wiki, and community tools](docs/community.md) |

The older [server.org](docs/server.org) path remains as a pointer. The
[API notes](api/README.org), [recent-page utility notes](api/recently-updated/README.md),
and [connection-list notes](pages/list-html-connections/README.md) explain the
retained programs and their limitations. They are not an unattended installer.

## About tilde.club

Tilde.club started in September 2014. Paul Ford wrote about
[how it began](https://medium.com/message/tilde-club-i-had-a-couple-drinks-and-woke-up-with-1-000-nerds-a8904f0a2ebf).
The [website](https://tilde.club) and [user wiki](https://tilde.club/wiki/) cover
joining and using the server. This repo covers operating one.

[Preguntas frecuentes en español](faq-esp.md).

## Contributing

Corrections, tested configs, and missing instructions are welcome. Include the
OS and software versions you used, what you changed, and how you checked the
result. Documentation fixes are useful contributions too.

Use example domains and accounts. Leave passwords, private keys, MFA secrets,
and members' personal information out of commits and issue attachments. Keep
production-specific settings distinct from advice for a new installation.

Existing utilities are licensed under the terms retained in [LICENSE](LICENSE)
and their source notices. This update preserves those files and attributions.
