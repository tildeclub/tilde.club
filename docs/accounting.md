# Accounting

This page collects simple command-line checks for understanding shell usage on a multi-user host.

## Longest cumulative login time (per user)

The historical one-liner below is still useful, but this version is easier to read and less dependent on shell backticks:

```bash
users | tr ' ' '\n' | sort -u | while read -r user; do
  ac "$user" | awk -v u="$user" '{print $1, u}'
done | sort -n
```

### Notes

- `ac` reads connection accounting data (typically from `/var/log/wtmp`).
- Results are cumulative and depend on log retention.
- On some systems, log rotation or reboots can make totals look lower than expected.

## Who is currently online

```bash
who
```

If you only need usernames:

```bash
who | awk '{print $1}' | sort -u
```

## Most recent login for each user

```bash
lastlog
```

This is useful for finding dormant accounts and checking recent activity.

## Total command usage summary (process accounting)

If process accounting is enabled, you can summarize command usage:

```bash
sa
```

Not all systems enable this by default; if `sa` has no data, verify process accounting configuration first.
