# Usenet and local newsgroups

The current shell host runs INN 2.7.3. Its reader rules, peer list, and spool
configuration were not supplied. The old Fedora 32 setup and manual history
rebuild have been replaced here with a local-first deployment procedure.

## Install and inspect the packaged layout

```bash
dnf install inn slrn
rpm -ql inn
systemctl cat innd.service
```

Use the config, service account, spool, history, and log paths supplied by that
package. Do not replace `pathnews` with `/usr/local/news` merely because an old
source-install guide used it. Keep a copy of the packaged examples and consult
the installed `inn.conf(5)` and `readers.conf(5)` manuals.

In `inn.conf`, set your own `organization`, `pathhost`, `domain`, and reader
`server` name. Retain the packaged storage/history layout until you have a
reason to change it. Keep the first deployment reachable only from loopback or
an explicit trusted network, using both the service's bind policy and the
firewall.

## Local reader policy

INN separates reader authentication from reader access. A small example
`readers.conf` for trusted localhost shell clients is:

```text
auth "shell-local" {
    hosts: "127.0.0.1,::1"
    default: "<shell-local>"
}
access "shell-local" {
    users: "<shell-local>"
    read: "local.*"
    post: "local.*"
}
```

This deliberately trusts connections originating on the machine and does not
authenticate an individual shell user. Decide whether that is sufficient for
your community. It is not an appropriate anonymous Internet posting policy.
Review the whole file: another broad auth/access block can grant access you
did not intend, and matching order matters.

Use the package's `inncheck` as the news service user, fix reported errors,
then start `innd`. Locate the packaged admin binaries if they are not on PATH:

```bash
rpm -ql inn | rg '/(inncheck|ctlinnd|news.daily)$'
systemctl enable --now innd
```

Create a disposable local group with the installed `ctlinnd newgroup` command,
for example `local.test`, as the news service user. Read, post, and retrieve a
test article with a local client. Check the configured spool and logs after a
restart. Test that a client outside the permitted source range is denied.

## Expiry and maintenance

Inspect the packaged expiry timer and cron integration before adding another
job. Configure `expire.ctl` for the groups and retention you intend to keep,
then verify `news.daily` actually runs. Monitor spool blocks and inodes, history
size, failures, and feed queues. A job in a document is not evidence that it is
scheduled on the host.

Back up the config and recovery-critical spool/history state according to the
storage method and upstream recovery procedure. Do not run an old `makehistory`
recipe against a live spool without confirming the storage method, paths,
backups, and downtime requirements. Never move guessed `.hash`, `.dir`, or
`.index` files into place because they appeared in a guide for another INN version.

## Remote readers and peering

Keep these separate. TLS/authenticated reader access has a different purpose
from server-to-server article feeds. Before adding a remote peer, agree on
names, source addresses, offered groups, moderation/control policy, volume,
spam handling, and feed authentication. Configure `incoming.conf` and
`newsfeeds` for those peers specifically. Do not use a wildcard peer rule or
open reader posting simply to make a connection test pass.

Use the current [INN documentation](https://www.eyrie.org/~eagle/software/inn/docs/),
including [reader access](https://www.eyrie.org/~eagle/software/inn/docs/readers.conf.html)
and [installation](https://www.eyrie.org/~eagle/software/inn/docs/install.html).
