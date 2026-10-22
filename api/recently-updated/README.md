# Recently updated pages: retained implementations

These scripts are kept so existing users do not lose the programs or old paths.
They are historical implementations, not a tested production installation
recipe. The maintained deployment source must be chosen and reviewed separately;
there is also [tildeclub/recently-updated](https://github.com/tildeclub/recently-updated).

## Files and interfaces

| File | Behavior |
| --- | --- |
| `findmod.sh` | Writes HTML to stdout for recently changed top-level `index.html` files. |
| `findmod-json.sh` | Writes a JSON-like `pagelist` response to stdout for the same index scan. |
| `findmod2.pl` | Scans each public subtree and writes both HTML and JSON output files. |
| `header.tpl`, `footer.tpl` | HTML fragments used by the original shell implementation's external template paths. |

The original shell script hard-codes template paths in another user's home and
an HTTP domain. Configure a reviewed copy for your own domain and deployment
path rather than creating that person's home layout on a new host.

The Perl program accepts `--domain`, `--root`, `--since-hours`, `--destdir`,
`--update-every`, `--verbose`, and `--help`. Its default window is 24 hours and
its output names are `tilde.24h.html` and `tilde.24h.json` (with the chosen hour
count substituted). A positive update period makes it keep rescanning.

## Limitations to fix before deployment

The retained implementations concatenate strings instead of safely serializing
all output. The Perl JSON writer includes a trailing comma, so its nonempty JSON
output is invalid. HTML escaping and URL encoding also need review. Filesystem
names must not be trusted as markup, and timestamps labeled UTC must actually
be generated in UTC. The shell regex also restricts which usernames it finds.

The tree scanner can traverse large user-controlled public trees. Define bounds,
symlink handling, public-file eligibility, error handling, and exclusion policy.
Run with read access only to intended public content; running it as root can
publish information about files the web server itself cannot read.

For a deployment, validate JSON with `python3 -m json.tool`, check HTML and URLs,
write to a private temporary file on the destination filesystem, and rename the
validated output into place. Do not truncate the live page at the start of a
scan or repeatedly publish partial files from overlapping jobs.

The original scripts were contributed by Jason Levine (~delfuego) and Chris
Reuter (~suetanvil). Their interfaces and code are preserved by this docs update;
no claim is made that they have been repaired or replaced here.
