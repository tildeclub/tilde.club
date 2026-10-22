# Running the shell host

Use [the setup guide](how-to-set-up-a-tilde.md) for a new machine. This chapter
covers the work after people start using it.

## Accounts and groups

Give each person a private primary group. Use supplementary groups for specific
shared resources. The production `makeuser` also adds a common group by numeric
ID; resolve that ID locally before copying the policy to another host.

Keep the list of privileged groups short. Membership in `wheel`, Docker's
administration group, or a service configuration group can be equivalent to
root access. Shared web backends need their own identities and narrowly scoped
permissions; see [web publishing](web.md).

To suspend a compromised account, disable its actual login paths. Locking the
Unix password alone does not necessarily prevent public-key SSH or existing
sessions. Check keys, SSH account rules, active sessions, cron, user services,
ZNC, web applications, and application-specific credentials. Preserve the data
needed to understand the incident before deleting anything.

Use [account provisioning](accounts.md) as a checklist. Provisioning a Unix
account and provisioning its external services are separate operations.

## Disk quotas

The collected host uses XFS with `usrquota,grpquota` on the root filesystem.
`/home` is a directory on that filesystem. The collector's attempt to query
`xfs_quota` with `/home` as a mount path failed even though the command returned
zero, so inspect its output as well as its exit status.

First discover the mount:

```bash
findmnt --target /home --output TARGET,FSTYPE,OPTIONS
```

On the recorded layout, query and set limits using `/`:

```bash
xfs_quota -x -c 'state' /
xfs_quota -x -c 'report -h -u' /
xfs_quota -x -c 'limit -u bsoft=1G bhard=3G member' /
xfs_quota -x -c 'quota -h -u member' /
```

These match the block limits requested by the current provisioning script.
The soft limit can be exceeded during a grace period; the hard limit cannot.
Check the grace policy and add inode limits if small-file counts are a concern.
If `/home` is a separate XFS mount on your host, use that mount instead.

Enabling XFS quotas can require mount or boot configuration work. Follow the
filesystem's documentation, take a backup, and arrange a maintenance window.
Do not remount a busy filesystem using an unreviewed command copied from here.
For ext4 or another filesystem, use its own quota tools and activation procedure.

Mail and user applications can live in the same quota as the home. Decide what
happens when someone reaches the hard limit: mail delivery and saving an editor
buffer are both affected.

## Process, memory, and CPU limits

The system record did not establish per-user limits. It reported an unlimited
aggregate `user.slice`, which alone says nothing about individual user slices
or PAM limits.

For a new host using systemd user slices, a starting example is
`/etc/systemd/system/user-.slice.d/50-tilde-limits.conf`:

```ini
[Slice]
TasksMax=512
MemoryHigh=1G
MemoryMax=2G
CPUQuota=200%
```

Those values are examples to test, not the existing server's measured policy.
The drop-in applies to `user-UID.slice` units. Apply configuration changes with
`systemctl daemon-reload`, then check a fresh test session:

```bash
id -u member
systemctl show user-UID.slice -p TasksMax -p MemoryHigh -p MemoryMax -p CPUQuotaPerSecUSec
loginctl user-status member
```

Replace `UID` with the number printed by `id`. Verify SSH sessions are actually
placed in that slice. A hard memory limit can terminate a user's work. Tell
people what the limits are and watch for repeated limit hits before tightening
them. PAM `limits.d` settings are useful for session limits but are not a
replacement for aggregate cgroup limits.

Run long-lived community daemons as separate service users with their own
limits. Enable user lingering only where persistent user services are intended.
Do not quietly turn every interactive account into an unrestricted boot-time
service account.

## Ports and firewall policy

The existing host uses CSF, with IPv4 and IPv6 rules and a number of historical
exceptions. Its broad port ranges are not a minimum firewall template. A new
host should allow the services it actually serves.

| Service | Typical inbound port | When it is needed |
| --- | --- | --- |
| SSH | TCP 22 | Member and admin shell access. |
| HTTP and HTTPS | TCP 80, 443 | Publishing and certificate validation. |
| SMTP delivery | TCP 25 | Receiving Internet mail. |
| Authenticated submission | TCP 587 or 465 | Sending mail from remote clients with TLS. |
| IMAPS | TCP 993 | Remote mailbox access. |
| ManageSieve | TCP 4190 | Remote filter editing, if enabled with appropriate TLS/auth. |
| Gopher | TCP 70 | Public Gopher service. |
| Gemini | TCP 1965 | Public Gemini service. |
| Finger | TCP 79 | A deliberately enabled finger service. |
| Ident | TCP 113 | Trusted IRC servers querying user identity. |
| NNTP/NNTP over TLS | TCP 119, 563 | Remote newsreaders, if intentionally allowed. |
| Mosh | UDP chosen from 60000–61000 | An explicit Mosh port allocation; the range is not a universal default. |

IRC client connections are outbound. You do not need an inbound IRC daemon port
just because members run an IRC client. Database, cache, PHP, CGI, and mail
filter sockets should remain local unless there is a separately designed reason
for remote access.

Check `ss -lntup`, the configured bind addresses, host rules, provider rules,
and an outside connection test. A listening socket is not proof of public
reachability, and a firewall listing is not proof of a working service.

## Updates and local programs

Install supported OS updates, read service migration notes, and schedule reboots.
For a shared host, announce downtime through a public status/contact path and
inside the shell. People may have a compiler or editor running when you reboot.

Keep an inventory of programs in `/usr/local`, services built from source,
Python virtual environments, Ruby gems, and application dependency lockfiles.
An RPM update will not update a different executable earlier in PATH. The
recorded borgmatic installation is a concrete example: cron uses
`/usr/bin/borgmatic` 2.1.9 while `/usr/local/bin/borgmatic` reports 1.5.21.

Do not update every language ecosystem globally as root on behalf of all users.
Separate OS packages, operator applications, and user-managed dependencies.
Keep root-owned service code outside user-writable directories.

## Routine checks

```bash
systemctl --failed
systemctl list-timers --all
ss -lntup
df -h
df -i
journalctl -p err --since today
```

Review mail queues, backup completion, certificate expiry, disk and inode use,
resource-limit events, and pending account requests. Keep logs rotated and
retain enough history to investigate failures without logging passwords or
private user content unnecessarily. See [accounting](accounting.md) for what
session and process accounting can actually tell you.

Document how to recover from a failed update, restore an account, rotate a
credential, handle an abuse report, and reach someone when the host is down.
See [backup and recovery](backups.md) for the restore procedure.
