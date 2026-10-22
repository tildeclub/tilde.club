# Connecting to IRC

The endpoint recorded for NewNet in this repo is:

| Setting | Value |
| --- | --- |
| Server | `irc.newnet.net` |
| TLS port | `6697` |
| Browser client | <https://web.newnet.net/?join=club> |

Use TLS and keep certificate validation enabled. `irc.newnet.net` is a network
name that can direct clients to different servers; it is not proof that the
IRCd runs on the shell host.

For a new community, put your own network/channel choice in the member README,
MOTD, and client defaults. Check the network's connection and registration
policy before making hundreds of accounts connect from one host. Do not ship
member passwords or a privileged bot credential in the skeleton.

See [IRC operations](ircserver.md) for clients, ident, ZNC, and separate network
hosting. Endpoint details should be rechecked when the operator changes the
network or connection policy.
