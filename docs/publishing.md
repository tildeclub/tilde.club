# Gopher, Gemini, and finger

These are separate services with separate public directories. Creating a
`public_gopher` or `public_gemini` directory does not make it reachable until
the corresponding service is configured and tested.

## Gopher with Fossor

The current host uses [Fossor](https://github.com/tildeclub/Fossor), with
systemd accepting TCP connections on port 70 and starting `fossor@.service` for
each connection. The effective collected settings are:

- `MaxConnections=128`, `MaxConnectionsPerSource=8` on the socket.
- Service UID/group `gopher`, a 300-second runtime limit, and a 10-second stop timeout.
- Options read from `/etc/default/fossor` and then `/etc/sysconfig/fossor`.

The values of those option files were not supplied. The new-host setup below
chooses its own root and initially disables executable maps/CGI. A proxy can
make every connection appear to come from one address; account for that before
copying a per-source connection limit.

### Build and install on a new host

The reviewed Fossor checkout provides release `v3.1.2`. Check the project's
release/security notes before installation and record the actual tag/commit:

```bash
dnf install gcc make git
git clone --branch v3.1.2 https://github.com/tildeclub/Fossor.git
cd Fossor
./configure --listener=systemd --hostname=tilde.example.net \
    --gopherroot=/srv/gopher --systemd=/etc/systemd/system
make
```

Review the generated files, then create a dedicated service identity and install
from that reviewed checkout:

```bash
useradd --system --user-group --no-create-home --shell /usr/sbin/nologin gopher
install -d -o root -g root -m 0755 /srv/gopher
make install
```

Run the privileged commands as root; do the source checkout and build as an
ordinary build account. `make install` installs the executable, options, and
units. It can replace existing Fossor files, so these commands are for a new
installation. For migration, use the project's migration notes and preserve
existing content and config first.

Set `OPTIONS` in `/etc/sysconfig/fossor` to a new-host configuration such as:

```bash
OPTIONS="-h tilde.example.net -r /srv/gopher -u public_gopher -nv -nx"
```

`-nx` disables executable content; `-nv` disables Gopher virtual hosting. Keep
one authoritative options value: the service reads both option files, with the
later environment file taking precedence for repeated variables.

Use `systemctl edit fossor@.service` for a local override:

```ini
[Service]
User=gopher
Group=gopher
RuntimeMaxSec=300
TimeoutStopSec=10
```

Create a root `gophermap` and one member's public directory. A menu file uses
literal tabs between link fields:

```bash
printf '!Our tilde\n~\n' > /srv/gopher/gophermap
install -d -o member -g member -m 0755 /home/member/public_gopher
printf '!My gopher space\nHello from my tilde.\n' > /home/member/public_gopher/gophermap
chown member:member /home/member/public_gopher/gophermap
chmod 0644 /srv/gopher/gophermap /home/member/public_gopher/gophermap
systemctl daemon-reload
systemctl enable --now fossor.socket
```

Allow TCP 70 in the chosen firewall manager, then test a Gopher client against
`gopher://tilde.example.net/` and `gopher://tilde.example.net/1/~member/`.
Use world-readable public content with traversable parent directories; Fossor's
public-file checks are stricter than merely allowing its UID to read a file.
Test that private files and traversal attempts are refused. Keep an executable
map test out of the live service until you deliberately enable execution and
understand which UID will run it.

Check `systemctl status fossor.socket` and `journalctl -u 'fossor@*'`. Keep a
version/update record for the manually installed binary; an OS package update
will not update it automatically.

## Gemini with gemserv

The collected service runs as `gemini`, with working directory
`/srv/gemini/gemserv` and this command:

```text
/srv/gemini/gemserv/target/release/gemserv config.toml
```

Its actual `config.toml` and source revision were not supplied. Do not infer
its vhosts, TLS keys, or execution policy from the unit alone.

For a new installation, review the
[upstream project](https://git.sr.ht/~int80h/gemserv), its dependency/update
status, and the example config shipped with the revision you choose. Build as
an ordinary account with the lockfile, using `cargo build --release --locked`.
If the chosen revision supports the static-only feature set, use
`--no-default-features` when that is all you intend to offer. Install the built
binary as root-owned `/usr/local/sbin/gemserv`; record its source revision.

The published gemserv config schema uses `interface`, `[[server]]`, `hostname`,
`dir`, `cert`, `key`, `index`, and **`usrdir`** for `~/public_gemini`. Use the
[small example](../examples/gemserv/config.toml) only with a revision supporting
that schema; it is not a copy of the uncollected live config.

Create a dedicated `gemini` system user/group, root-owned public content at
`/srv/gemini/content`, and a root-owned config directory at `/etc/gemserv`.
Create the new-host service identity and paths before installing its config:

```bash
useradd --system --user-group --no-create-home --shell /usr/sbin/nologin gemini
install -d -o root -g gemini -m 0750 /etc/gemserv
install -d -o root -g root -m 0755 /srv/gemini/content
```

A self-signed Gemini certificate for the example hostname can be made with:

```bash
openssl req -x509 -newkey ec -pkeyopt ec_paramgen_curve:P-256 -sha256 \
    -days 365 -noenc -subj '/CN=tilde.example.net' \
    -addext 'subjectAltName=DNS:tilde.example.net' \
    -keyout /etc/gemserv/key.pem -out /etc/gemserv/cert.pem
chown root:gemini /etc/gemserv/key.pem
chmod 0640 /etc/gemserv/key.pem
chmod 0644 /etc/gemserv/cert.pem
```

Gemini clients commonly keep certificate fingerprints. Record your replacement
and renewal policy so a changed certificate can be explained. Do not loosen the
permissions on a shared HTTPS private key to make a different service work.

Install [the example unit](../examples/gemserv/gemserv.service) as
`/etc/systemd/system/gemserv.service`, after creating its user and paths. Add
`index.gmi` under the main content root and a member's `public_gemini` at 0755,
with public files at 0644. Load the unit and start it:

```bash
systemctl daemon-reload
systemctl enable --now gemserv
journalctl -u gemserv --since today
```

Open TCP 1965 only when ready. Test the root and the member's URL with a Gemini
client, both externally and over IPv6 if advertised. Also test missing paths,
private files, symlinks, encoded traversal, and the deliberately disabled CGI
policy. Set timeouts and resource controls using the chosen version and the
service manager; do not assume a web server's limits apply to Gemini.

## Finger

The current host runs a local Python `fingerd.py` as `efingerd`, with only the
bind-service capability needed to listen on a privileged port. Its source was
not part of the collection, so this guide does not invent its response format
or access policy.

Finger is optional. Before offering it, decide which account/profile fields
are public, how members opt out, what the response says about logged-in users,
and how much output or request volume you allow. A public finger response
should not disclose private files just because the daemon can read them.
