When we're ready to allow other tilde.club IRC servers to connect to us, these are we'll need bits of information from the remote server admin.

* remote IRC server IP address (if the server is behind NAT, this needs to be the external IP address)
* port the server is listening on (specifically the SSL port; we should only support SSL connections)
* the name of the remote IRC server (if it's a charybdis server, this is `name` in the `serverinfo` section of its `ircd.conf` file)
* two passwords, one we will send to the remote server, and one we will receive from the remote server

The remote server should choose an [SID](http://www.stack.nl/~jilles/irc/charybdis-oper-guide/configlines.htm) (one digit and two characters which can be digits or letters); these need to be unique across the peer network. (See the bottom of this page for the current list in our peer network.) This SID then goes into the `serverinfo` block of the remote server's `ircd.conf` file.

The remote server must either be running a locally-caching DNS server (a la `dnsmasq`) or be using an authoritative DNS server that maps their tilde hostname (e.g., tilde.club) to whatever IP address their clients use to connect to their IRC server, since charybdis and its ancestors are DNS-bound for their resolution of who they are. (For most of the other tilde boxes, they're running a single server combining both shell and IRC services, and their users are connecting to IRC via localhost -- which means that when they peer with us, their local clients appear to be "whatever@127.0.0.1". We want to know the real host that clients are connected to.) So if the remote server is using `dnsmasq`, they need to map 127.0.0.1 to their tilde hostname (e.g., `127.0.0.1 other.tilde.host localhost` in their `/etc/hosts/` file).

Finally, the remote server should be running an identd daemon (a la `oidentd`) on whatever host their clients have shell accounts on -- again, it's usually the same host. And again, this is because IRC is the one service out there that really, really tries to use identd to determine the non-spoofed username of the user connecting to it.

Then, these are the configurations changes that we'll need to make at our end to enable the connections.

### Firewall or EC2 security group

The main tilde.club IRC server needs to allow traffic from the remote server; this means that we need to allow traffic from the specific IP address toport `6697` (SSL connections).

### charybdis ircd.conf file

There needs to be [a `connect` section](https://github.com/tildeclub/tilde.club/blob/master/docs/ircserver.md#connect-section) in the `ircd.conf` file for each remote server. See [the documentation for our server setup](https://github.com/tildeclub/tilde.club/blob/master/docs/ircserver.md#connect-section) for full information about how this is configured.

## Debugging the connections

You should have the `/var/log/charybdis/serverinfo` log file enabled, and it's this file that will contain the pertinent information about the success or failure of server connections.

## Current peered hosts and server SIDs

* tilde.club (us): `01A`
* yester.host: `42y`
* tilde.red: `42X`
