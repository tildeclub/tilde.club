# Login and process accounting

Accounting is useful for understanding use and investigating a problem. It is
not a reliable count of people and it is not proof of what someone intended.
Tell members what you retain and restrict access to logs containing addresses
and commands.

## Current sessions

```bash
who
w
loginctl list-sessions
```

These commands answer different questions. A persistent tmux session, a user
service, and a current terminal login are not the same thing. Some terminal
programs also update traditional login records differently.

## Login history

On systems with traditional wtmp records:

```bash
last
ac -p
```

`ac -p` summarizes the retained connection records for all recorded users.
Do not start with `users` if the goal is to count everybody: that limits the
input to people logged in at the time you run the command. Rotated or missing
records change the totals, and concurrent sessions are not unique human time.

Check which tools and storage the OS actually uses. Newer Fedora installations
may use lastlog2 and different login-record implementations. Use the installed
`lastlog2 --help` and service documentation rather than assuming the old
`lastlog` file is populated. A missing record does not establish that an account
has never been used.

## Process accounting

The collected host has `psacct.service` active. On a new Fedora host, install
and enable it only after deciding the logging and retention policy:

```bash
dnf install psacct
systemctl enable --now psacct
sa
```

`sa` summarizes completed-process accounting records. It does not replace
systemd resource limits, live process inspection, or application logs. Check
package log rotation so accounting data does not consume the filesystem.

For a service problem, start with that service's journal and resource counters.
For a user resource problem, inspect the relevant user slice as described in
[shell operations](shellserver.md#process-memory-and-cpu-limits).
