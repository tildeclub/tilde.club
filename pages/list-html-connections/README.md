# Connection-list utility

`social.sh` and `ack2list.pl` are retained historical utilities. The shell
pipeline uses `ack` to find local tilde links, then the Perl script generates an
HTML table. `css/social.css` supplies the stylesheet.

The original pipeline assumes a working directory containing `ack2list.pl` and
an output directory named `public_html`. It scans `/home`, has hard-coded
exclusions, and does not safely escape all filesystem/link values as HTML.
It is not an appropriate root job or a web-request handler on a new shared host.

Before using a reviewed copy, restrict scanning to intended public HTML,
handle permissions and symlinks, bound the scan, validate/escape the output,
and publish atomically from staging. Decide whether indexing is opt-in and
how members exclude a page. Preserve other people's private files and do not
interpret local links as shell commands.

The docs refresh keeps the utility and stylesheet intact for existing users.
See [community tooling](../../docs/community.md) for the current deployment
approach and [the setup guide](../../docs/how-to-set-up-a-tilde.md) for the host.
