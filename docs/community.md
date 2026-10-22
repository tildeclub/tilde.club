# Website, signups, and community tools

A working shell and a place to publish are enough to start. Add community tools
when someone will maintain them. Each operator application needs its own code,
service identity, writable data, update procedure, and backup entry.

## Source repositories

These public repos provide parts of the tilde.club setup. Review the code and
its revision before deployment; a public checkout is not necessarily identical
to a local production copy.

| Component | Source |
| --- | --- |
| Website and wiki | [tildeclub/site](https://github.com/tildeclub/site) |
| Home skeleton | [tildeclub/tilde.etcskel](https://github.com/tildeclub/tilde.etcskel) |
| Account creation | [tildeclub/makeuser](https://github.com/tildeclub/makeuser); the collected live script has local integrations. |
| First-login wizard | [tildeclub/welcome](https://github.com/tildeclub/welcome) |
| Gopher daemon | [tildeclub/Fossor](https://github.com/tildeclub/Fossor) |
| Gopher root content | [tildeclub/gopherhole](https://github.com/tildeclub/gopherhole) |
| Gemini root content | [tildeclub/geminipage](https://github.com/tildeclub/geminipage) |
| MOTD | [tildeclub/motd](https://github.com/tildeclub/motd) |
| Terminal wiki helper | [tildeclub/wiki-cli](https://github.com/tildeclub/wiki-cli) |
| Recent-page listing | [tildeclub/recently-updated](https://github.com/tildeclub/recently-updated) |
| Botany | [tildeclub/botany](https://github.com/tildeclub/botany) |
| Terminal blogging | [tildeclub/ttbp](https://github.com/tildeclub/ttbp) |
| Forum client | [tildeclub/ttrv](https://github.com/tildeclub/ttrv) |
| Helpdesk | [tildeclub/helpdesk](https://github.com/tildeclub/helpdesk) |
| File sharing | [tildeclub/share](https://github.com/tildeclub/share) |

## Deploy the website deliberately

Stage `tildeclub/site` outside the web root, select and record a revision, then
review domains, contact addresses, signup behavior, JSON data, helper scripts,
and web dependencies. The collected host's main root is
`/usr/share/nginx/html`; configure its own PHP handler before deploying a PHP
front page. The static bootstrap example deliberately does not execute PHP.

Keep `.git`, credentials, local environment files, Composer's development
artifacts, and diagnostics out of the served copy. In particular, the reviewed
site checkout has a signup `_probe.php` mail diagnostic; do not deploy that as
a public endpoint. Install each application's declared dependencies using its
lockfile in a build/staging location and check its supported PHP version.
Do not infer a database requirement merely because MariaDB runs on the machine.

Make code root-owned and grant the backend write access only to the files it
must update. Avoid making the entire document root writable by nginx or giving
an operator application the same writable identity as every member's PHP.
Test error pages, forms, mail submission, and file ownership on the target host.

The wiki has its own `source/` Markdown and Makefile in the site repo. Review
that build file, install its declared tools, and build in staging. Check that
generated pages, local links, feeds, and navigation are present before syncing
the output. A successful checkout is not a completed wiki deployment.

## Signup requests are data

The reviewed website sends a request to the operator and records pending
usernames and a suggested `makeuser` command in files beneath `/var`. The
collected account script later removes the processed pending entry. Neither
step is a transaction across the Unix account, mail, ZNC, helpdesk, and signup
queue.

For a new host, reserve service/admin names, validate the username and contact
address, parse the supplied SSH public key with OpenSSH, and review the request.
A useful key-file check is:

```bash
ssh-keygen -lf /root/pending/member.pub
```

Keep that pending directory private. Construct privileged invocations from
validated arguments or structured records. Do not paste an emailed shell
command into a root terminal without inspecting its quoting and values, and do
not execute a web-writable queue as a shell script.

The public form should handle duplicate requests, concurrent writes, invalid
keys, rate limiting, CSRF where applicable, and mail failures. Trust forwarded
client-address headers only from a configured trusted proxy. Keep request data
and contact addresses out of the document root and define their retention.
Running a signup form is not a reason to give the web process passwordless sudo.

## First contact and support

Give the member a short welcome with:

- The hostname, username, SSH instructions, and host-key verification path.
- Where their pages live and which publishing services are enabled.
- First-login/MFA instructions and a recovery contact.
- Chat, mail, quotas, rules, and how to request help or packages.
- What account data is public and how to request removal.

The existing script emails a generated password and copies it to local mailboxes.
That is its recorded behavior, not a requirement for a new installation. Choose
an enrollment process consistent with your credential and mailbox-retention
policy, and ensure temporary credentials are changed before normal use.

Keep MOTD concise. Put longer help in the wiki or a local `help` command, and
publish an independent status/contact path for when the host is unavailable.
Respect the difference between a maintenance notice and a public announcement
of a new member's account; make the announcement policy clear.

## Optional apps

Install apps under separate operator-managed paths and identities. Record the
revision and dependency lockfile, local socket or port, reverse-proxy route,
backup data, and a restart/restore test. If an app requires secrets, load them
from protected config or the service environment, never from a public checkout.

For cgit, only expose repositories deliberately marked public, validate the user
and repository mapping, and test path traversal. For Botany and similar shared
apps, test whose files the backend can read and write. Keep IRC bot rate limits
and reconnection behavior within the network's policy. Treat browser terminals
and relay sockets as authenticated services, not harmless static content.

The older utilities retained under `api/` and `pages/` are not used as an
automatic install system. Their current limitations and safer deployment rules
are documented in [the API notes](../api/README.org) and
[connection-list notes](../pages/list-html-connections/README.md).
