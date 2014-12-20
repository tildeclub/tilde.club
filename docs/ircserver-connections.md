When we're ready to allow other tilde.club IRC servers to connect to us, these are we'll need bits of information from the remote server admin.

1. remote IRC server IP address (this needs to be the externally-reachable IP address)
1. port the server is listening on (specifically the SSL port; we only support SSL connections)
1. the name of the remote IRC server (if it's a charybdis server, this is `name` in the `serverinfo` section of its `ircd.conf` file)
1. two passwords, one we will send to the remote server, and one we will receive from the remote server

The remote server must choose an [SID](http://www.stack.nl/~jilles/irc/charybdis-oper-guide/configlines.htm) (one digit and two characters which can be digits or letters); these need to be unique across the peer network. (See the bottom of this page for the current list in our peer network.) This SID then goes into the `serverinfo` block of the remote server's `ircd.conf` file.

IRC is highly DNS-dependent; the remote IRC server must be using a DNS server which can resolve hostnames for its clients' IP addresses. So that means that if the IRC server is running on the shell server box, and clients will be connecting to `localhost`, then the DNS server needs to be able to resolve `127.0.0.1` to its valid tilde hostname; if it can't, then clients will appear as *username@127.0.0.1* rather than *username@tildehost.tld*. We're running `dnsmasq`, which is super-lightweight and uses `/etc/hosts` for its configuration -- so we were able to just add a line such as `127.0.0.1 tildehost.tld localhost` in our `/etc/hosts` and everything worked as it shoudl.

Finally, the remote shell server should be running an identd daemon (a la `oidentd`). This is because IRC is the one service out there that really, really tries to use identd to determine the non-spoofed username of the user connecting to it.

Then, these are the configurations changes that we'll need to make at our end to enable the connections.

### Firewall or EC2 security group

The main tilde.club IRC server needs to allow traffic from the remote server; this means that we need to allow traffic from the specific IP address to port `6697` (SSL connections).

### charybdis ircd.conf file

There needs to be [a `connect` section](https://github.com/tildeclub/tilde.club/blob/master/docs/ircserver.md#connect-section) in the `ircd.conf` file for each remote server. See [the documentation for our server setup](https://github.com/tildeclub/tilde.club/blob/master/docs/ircserver.md#connect-section) for full information about how this is configured.

## Debugging the connections

You should have the `/var/log/charybdis/serverinfo` log file enabled, and it's this file that will contain the pertinent information about the success or failure of server connections.

## Current peered hosts and server SIDs

* tilde.club (us): `01A`
* yester.host: `42y`
* tilde.red: `42X`
* tilde.works: `42Z`
* irc.club6.nl: `6NL`
