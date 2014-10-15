# Setting up the tilde.club IRC server

We chose to go with `charybdis`, mostly because it's in the Debian/Ubuntu package repository (universe), so we can update it without having to go through the `configure`/`make`/`make install` process.

> FYI, we've done a bit of testing, and aren't sure that `charybdis` will interconnect server-to-server with `ngircd`, another open-source IRC daemon that's available out there. So if you're setting up a potential IRC peer of the tilde.club IRC server, this might be an important bit of information to know.

## System setup

The IRC server is an Amazon EC2 instance running Ubuntu. Initial system setup on the IRC server included:

* Setting the hostname (`sudo hostname irc`) (and perhaps also needing to edit the `/etc/hostname` file -- unclear)
* updating the `/etc/hosts` file (add `172.30.0.216 irc.tilde.club irc` and `172.30.0.176 tilde.club tilde`)
* setting the EC2 security group to allow TCP traffic on ports 6667 and 6697 from the `tilde.club` shell server only

### Setup changes on the tilde.club shell server to access the IRC server

The `tilde.club` shell server needed a few tweaks as well:

* updating `/etc/hosts` (add `172.30.0.216 irc.tilde.club irc` so users can reach the IRC server by hostname)
* setting the EC2 security group to allow TCP traffic on port 113 (`identd`) from the IRC server only

## Installing charybdis

First we needed to add the "universe" repository to the `apt` repository list:

```
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu $(lsb_release -sc) universe"
```

Once that was added, `charybdis` could be installed:

```
sudo apt-get install charybdis
```

## Configuring charybdis

We first needed to set up SSL certs, which are good for local users who want to use SSL, but also will be important if and when we allow linked tilde.club IRC servers.

```
sudo /usr/bin/charybdis-genssl
```

I'm not sure if it's critical, but I set the cert's common name to the same name I chose for the IRC server (`irc.tilde.club`); I answered the rest of the questions logically (`US`, `DC`, `Washington`, `tilde.club` for the org name, `delfuego@tilde.club` for the contact email).

The next step was to edit the `/etc/charybdis/ircd.conf` file. There are [a bunch of settings](http://www.stack.nl/~jilles/irc/charybdis-oper-guide/index.htm) here, organized into sections; these are the important ones.

### serverinfo section

* **name**: the same name you used above for the common name of the SSL cert [`irc.tilde.club`]
* **sid**: a unique identifier, two digits and a character; these distinguish networked servers from each other
* **description**: simple
* **network_name**: I actually don't think this matters [`tilde.club`]
* **network_desc**: I also don't think this matters [`your very own network!`]
* **hub**: determines whether servers can connect at all [`yes`]
* **ssl_private_key**: location of the private key you generated above [`/etc/charybdis/ssl.key`]
* **ssl_cert**: location of the SSL cert [`/etc/charybdis/ssl.cert`]
* **ssl_dh_params**: location of the SSL Diffie-Hellman parameters file [`/etc/charybdis/dh.pem`]

### admin section

This info is reported when an IRC user issues the `/INFO` command, so you might want it to be accurate.

### log section

The defaults here are meaningless, since they're relative paths and I can't figure out what they're relative *to*. So I changed all these to absolute paths, such as:

* **fname_userlog**: `/var/log/charybdis/userlog`

Another important note: it appears that the file has to exist and be writeable by charybdis in order for it to be used, so for each log file you enable, the following needs to be done:

```
sudo touch /var/log/charybdis/userlog
sudo chown charybdis.charybdis /var/log/charybdis/userlog
sudo chmod 644 /var/log/charybdis/userlog
```

Finally, the `serverlog` file is the relevant one for debugging server-to-server connections, so that was an important one to enable.

### listen section

* **port**: port 6667 is the typical IRC server port [`5000, 6665 .. 6669`]
* **sslport**: port 6697 is the typical IRC SSL server port [`6697`]

### auth section

There can be multiple `auth` sections, but there can't be sections that have identical `user` entries.

* **user**: the mask for users allowed to connect [`*@172.30.0.0/16`]
* **class**: the class that matching users are put into [`users`]

### operator section

Any users who should be able to elevate with `/OPER` need a section here; these sections are different in that the section head includes the IRC nickname of the user (e.g., `operator "ubuntu" {`).

* *header*: see above, you need to include the nickname in the header
* **user**: the full hostmask of the user [`delfuego@172.30.0.176`]
* **password**: the password the user needs to use after issuing `/OPER`; it can be encrypted using `mkpasswd` or left unencrypted
* **flags**: either `encrypted` if the password is encrypted, or `~encrypted` if it isn't [`encrypted`]

### connect section

This section is only needed if you're allowing other servers to connect and/or connecting to other servers. It's like the `operator` section in that the header includes an additional string, the name of the server to which you're connecting.

* *header*: see above, this needs to be the exact name -- as in, the `name` in the `serverinfo` section -- of the server you're connecting to (or is connecting to you)
* **host**: the IP address of the remote server
* **send_password**: this is the password you'll send to the remote server (configured as that server's `accept_password`)
* **accept_password**: this is the password you'll accept from the remote server (configured as that server's `send_password`)
* **port**: the port you'll connect to on the remote server
* **hub_mask**: the mask of hosts that a hub can introduce to each other [`*`]
* **class**: [`server`]
* **flags**: various flags for server-to-server behaviors [`compressed, topicburst, ssl`] *There's also an `autoconn` flag, and one of the two servers should have it enabled; I think it's good practice for us to expect the remote server to autoconnect to us, not vice-versa.*
