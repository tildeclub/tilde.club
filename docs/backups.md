# Backup and recovery

The collected host runs Borg 1.4.5. Root's crontab invokes
`/usr/bin/borgmatic` at 03:00 daily; that executable reports 2.1.9. A separate
`/usr/local/bin/borgmatic` reports 1.5.21 and appears earlier in the collector's
PATH. Use the absolute path from the actual job when checking versions,
validating configs, or reproducing a run.

The production backup configuration and credentials were not collected. The
example here is a new-host plan, not a claim about its repository, retention,
or current source coverage. Historical backup logs are also kept on the
operator's NAS; missing local journal history is not proof a backup did not run.

## Decide what must be recoverable

Include these in the plan:

| Data | What to capture |
| --- | --- |
| Accounts | `/etc`, homes, UIDs/GIDs, keys, shell defaults, quotas, and group memberships. |
| Member publishing and mail | Public directories, private mailbox data, forwarding and Sieve files. |
| Local code | `/usr/local`, source revisions, application lockfiles, service definitions, and build notes. |
| Operator web apps | Code, uploaded content, config, and a consistent database backup. |
| Service state | ZNC, INN spool/history, Gemini/Gopher content, and other app-specific state. |
| Secrets | Restricted encrypted copies of signing keys, service credentials, and recovery material. |
| Backup recovery | Repository key, passphrase, SSH credentials, destination, and restore instructions held off the host. |

Do not assume all state lives in `/home`. Inspect service units and application
configs. A Postfix queue restore needs its own policy because re-delivering
queued mail can duplicate messages. Reinstalling a database package and copying
its live data directory is not a substitute for a supported database restore.

## Prepare a remote repository

Install the maintained `borgbackup` and `borgmatic` packages. Configure a
dedicated backup account/key on the destination and verify the host key through
a trusted channel. Restrict the remote key to the intended repository and
operations using the destination's supported Borg setup.

Keep the repository passphrase in a root-only file outside the repo. Create
`/root/.config/borg` at 0700 and the passphrase file at 0600 using a root editor
or password-manager procedure; do not put the passphrase in a shell command,
git commit, or public config archive.

For the Borg **1.x** example, initialize a new repository from a root Bash shell:

```bash
export BORG_PASSCOMMAND='cat /root/.config/borg/passphrase'
repository='ssh://backup@backup.example.net/./tilde'
/usr/bin/borg init --encryption=repokey-blake2 "$repository"
/usr/bin/borg key export "$repository" /root/tilde-borg-key-export
```

Store the export and the passphrase off the source host through a protected
channel. Test access from a recovery machine. Do not initialize a repository
that is already in use, and do not substitute Borg 2 commands or formats into a
Borg 1 backup without planning that migration.

## Configure borgmatic

Review [config.yaml](../examples/borgmatic/config.yaml). Put the adjusted file
in `/etc/borgmatic/config.yaml` with root ownership and mode 0600. It uses the
flat borgmatic 2.x schema, not the old 1.5 `location/storage/retention` sections.
Create the database-dump directory at mode 0700 before validation:

```bash
install -d -o root -g root -m 0700 /var/backups/tilde-databases
/usr/bin/borgmatic config validate
```

Add existing service-state directories to `source_directories`. Keep the
missing-source check enabled so a typo or missing mount fails visibly. If a
required data mount can disappear while leaving an empty mountpoint behind,
add a mount-identity check too: merely testing that a directory exists is not
sufficient.

The example keeps daily, weekly, and monthly restore points. Choose retention
based on available storage and the longest period a bad change might go
unnoticed. A live filesystem backup is not a simultaneous snapshot of every
file. Quiesce services or use consistent snapshots where that matters.

## Consistent databases

Use borgmatic's supported database hooks for your installed version, or prepare
logical backups before running it. Here is a small wrapper for a host with a
local MariaDB instance. Save it as `/usr/local/sbin/backup-tilde`, root-owned and
mode 0700, and adjust the database section if MariaDB is not part of your host:

```bash
#!/usr/bin/env bash
set -euo pipefail
export PATH=/usr/sbin:/usr/bin:/sbin:/bin
umask 077

dump_dir=/var/backups/tilde-databases
install -d -m 0700 "$dump_dir"
dump_file=$(mktemp "$dump_dir/.mariadb.XXXXXXXX.sql")
trap 'rm -f -- "$dump_file"' EXIT

mariadb-dump --all-databases --single-transaction --routines --events \
    --triggers --hex-blob > "$dump_file"
mv -f -- "$dump_file" "$dump_dir/mariadb.sql"

/usr/bin/borgmatic create prune compact check --verbosity 1
```

This assumes a local root socket login or a separately protected client config;
it does not embed a database password. `--single-transaction` gives a consistent
view of transactional tables, not arbitrary nontransactional tables or concurrent
schema changes. Check engines and use a suitable lock, snapshot, or maintenance
window where needed. A failed dump must stop the backup rather than silently
reuse yesterday's SQL.

For SQLite, use its backup API or the CLI `.backup` operation while the app is
running. Do not copy a database file while ignoring an active WAL. For other
databases, use their supported consistent export/snapshot method.

## Schedule and monitor

Run the wrapper manually once and inspect the resulting archive. For a
cron-based setup, a root crontab entry can be:

```cron
0 3 * * * /usr/bin/flock -n /run/tilde-backup.lock /usr/local/sbin/backup-tilde >> /var/log/tilde-backup.log 2>&1
```

Create the log at mode 0600 and configure root-only log rotation. This example
runs at 03:00 in cron's configured timezone; record that timezone. Do not enable
a second timer for the same job without intending two schedules. A skipped
lock acquisition is not a successful backup and needs to be visible in monitoring.

Alert when there is no successful archive within the expected interval, not
only when the scheduler launches a command. Check off-host capacity, archive
age, consistency checks, repository access, database exports, and restore tests.
Store monitoring/notification credentials separately from the public docs.

## Restore an account in isolation

On a recovery machine or isolated directory, use the matching Borg version and
repository key. With `BORG_PASSCOMMAND` set as above:

```bash
repository='ssh://backup@backup.example.net/./tilde'
/usr/bin/borg list "$repository"
install -d -m 0700 /root/tilde-restore-test
cd /root/tilde-restore-test
/usr/bin/borg extract --dry-run "$repository::ARCHIVE" home/member
/usr/bin/borg extract "$repository::ARCHIVE" home/member
```

Replace `ARCHIVE` with the exact archive name returned by the list command.
Check the stored path names first. Extract relative paths into the test
directory; do not start a restore by extracting over `/home` or `/etc`.

Verify file contents, private modes, ownership, symlinks, mail, public pages,
and database exports. Preserve numeric UIDs/GIDs or deliberately map them before
returning restored data to service. Reapply SELinux labels and re-create quota
limits on the restored filesystem. Test login on an isolated host, not by
announcing a restored production account before it has been checked.

For a whole-host restore, reinstall a supported OS, restore account identity
and data deliberately, restore databases using their matching tools, restore
service secrets, then enable services in dependency order. Keep outbound mail,
IRC bots, signup announcements, and scheduled jobs disabled until duplicate
external actions have been ruled out. Change DNS or reopen public access only
after the replacement has passed its service checks.

Record the last successful restore date, what was restored, and what failed.
A recovery key kept only inside the encrypted backup cannot recover that backup.
