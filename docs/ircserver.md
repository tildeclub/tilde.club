# Setting up the tilde.club IRC server

We chose to go with charybdis, mostly because it's in the Debian/Ubuntu package repository (universe), so we can update it without having to go through the configure/make/make install process.

## System setup

The server is an Amazon EC2 instance, running Ubuntu. Initial system setup included:

* modifying the hostname (`sudo hostname irc`) (and perhaps also needing to edit the `/etc/hostname` file, unclear)
* updating the `/etc/hosts` file (add `172.30.0.216 irc.tilde.club irc` and `172.30.0.176 tilde.club tilde`)
* setting the EC2 security group to allow TCP traffic on port 6667 from the tilde shell server only

### Setup changes on the tilde shell server to support the IRC server

The tilde shell server needed a few tweaks, as well:

* updating `/etc/hosts` (add `172.30.0.216 irc.tilde.club irc` so users can get to the server by hostname alone)
* setting the EC2 security group to allow TCP traffic on port 113 (identd) from the IRC server only

## Installing charybdis

First, we needed to add the "universe" repository to the `apt` repository list:

```
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu $(lsb_release -sc) universe"
```

Once that was added, charybdis could be installed:

```
sudo apt-get install charybdis
```
