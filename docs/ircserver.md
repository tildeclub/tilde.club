# IRC on a shared shell host

Members can use IRC without running an IRC network. Start with clients and a
clear place to meet. [Connection details](ircserver-connections.md) describe
the endpoint recorded in this repo.

The host inventory confirms oidentd and ZNC. It does not establish where the
network servers run or which daemon they currently use. The old EC2/Charybdis
instructions and private address map have been removed because they were not a
current deployment record.

## Clients and persistent sessions

Install maintained client packages such as Irssi or WeeChat. Provide a small
example using TLS with certificate validation. Let members keep their own
nickname/account credentials private; do not put a shared NickServ password in
`/etc/skel`.

A client inside tmux or Byobu can remain connected while the member disconnects
from SSH. A bouncer such as ZNC is a separate service that keeps IRC connections
and can replay buffered messages to a remote client. Document which option you
support instead of promising that every account includes both.

## Ident

A shared host can send many connections to the same IRC network from one IP.
Ident can help an IRC network identify individual local accounts, but it is not
a replacement for IRC account authentication. Agree on connection limits and
ident policy with the network's operators before inviting a large group.

On a new Fedora host, inspect the maintained `oidentd` package and its config,
then enable the intended service/socket. Restrict inbound TCP 113 to the IRC
servers that need it where practical. Decide whether users may override their
ident response, and verify IPv4 and IPv6 independently. Do not open a privileged
service just because the old guide had a `chkconfig` command for it.

## ZNC

The production `makeuser` invokes `znccreate.py` to create a ZNC account with a
three-network limit. It passes the generated password on stdin. The provisioning
script's existence does not establish the current ZNC listener, module set, or
account-recovery policy; those need their own config and tests.

For a new host, run ZNC under a dedicated service identity, with its writable
state outside the web root. Use the maintained package or a reviewed build,
configure a TLS listener, protect credentials and buffers, and set reasonable
network/buffer limits. Bind any administrative listener locally or restrict it
to the intended admin path. Test reconnects and account removal as well as the
initial login. Back up its config and state consistently.

Never hand a signup form permission to execute arbitrary root commands in
order to create a bouncer account. Keep the member-facing request and privileged
provisioning steps separate, with validated inputs and an auditable result.

See the [ZNC administration documentation](https://wiki.znc.in/ZNC).

## If you want to run an IRC server

Choose the daemon based on the network you intend to operate or join. IRC
client protocol compatibility does not imply server-link compatibility between
daemons. Linking needs agreement with that network's operators.

For a separate server deployment, document its supported version, source/package,
service user, hostname, TLS certificate renewal, listener policy, account services,
operator access, connection/flood limits, and backup/restore procedure. Keep
server-link secrets and operator credentials outside this public repo. Test the
config with the daemon's own checker and test a client before opening the port.

An independently administered IRC service should not need write access to the
shell host's homes or account database. The shell host's firewall only needs
outbound client access and whatever ident/bouncer policy you deliberately offer.
