# Setting up the tilde.club shell server

We want to document the ins and outs of setting up the server, so that others who're interested can learn (and help!).

## System setup

This part is for @ftrain.

## Email

The default MTA on CentOS is `postfix`. Our goal was to have a localhost-only mail service, which required that we configure postfix to listen only to localhost, and to bounce any emails which local users try to send off-server. Both configuration changes are handled in `/etc/postfix/main.cf`.

* the `inet_interfaces` parameter should just be localhost (`inet_interfaces = localhost`)
* the `default_transport` parameter should be the bounce message we want (so add `default_transport = error: outside mail is not deliverable` to the bottom of the file)

## Identd

If users are going to connect from their shell account to an IRC server, then it's *very* handy to have an identd server running. For us, that just meant installing the standard CentOS identd server, and then configuring it to startup automatically:

```
sudo apt-get install identd
sudo /etc/init.d/oidentd start
sudo chkconfig oidentd on
```
