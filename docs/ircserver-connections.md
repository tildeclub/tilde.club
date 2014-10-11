When we're ready to allow other tilde.club IRC servers to connect to us, these are we'll need bits of information from the remote server admin.

* remote IRC server IP address (if the server is behind NAT, this needs to be the external IP address)
* port the server is listening on (preferably the SSL port)
* the name of the remote IRC server (if it's a charybdis server, this is `name` in the `serverinfo` section of its `ircd.conf` file)
* two passwords, one we will send to the remote server, and one we will receive from the remote server

Then, these are the configurations changes that we'll need to make at our end to enable the connections.

### Firewall or EC2 security group

The main tilde.club IRC server needs to allow traffic from the remote server; this means that we need to allow traffic from the specific IP address to either port `6667` (for non-SSL connections) or port `6697` (for SSL connections).

### charybdis ircd.conf file

There needs to be [a `connect` section](https://github.com/tildeclub/tilde.club/blob/master/docs/ircserver.md#connect-section) in the `ircd.conf` file for each remote server. See [the documentation for our server setup](https://github.com/tildeclub/tilde.club/blob/master/docs/ircserver.md#connect-section) for full information about how this is configured.

## Debugging the connections

You should have the `/var/log/charybdis/serverinfo` log file enabled, and it's this file that will contain the pertinent information about the success or failure of server connections.
