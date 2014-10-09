# Setting up the tilde.club shell server (user host)

We want to document the ins and outs of setting up the server so others who are interested can learn (and help!).

## System setup

This part is for @ftrain.

## Email

The default MTA on CentOS is `postfix`. Our goal was to have a `localhost`-only mail service, which required that we configure `postfix` to listen only to `localhost`, and to bounce any email which local users try to send off-server. Both configuration changes are handled in `/etc/postfix/main.cf`.

* the `inet_interfaces` value should just be `localhost` (`inet_interfaces = localhost`)
* the `default_transport` parameter should be the bounce message we want (so add `default_transport = error: outside mail is not deliverable` to the bottom of the file)

## identd

Users will connect from their shell account to an IRC server, so it is *very* handy to have an `identd` server. For us that just meant installing the standard CentOS `identd` server and configuring it to start automatically:

```
sudo apt-get install identd
sudo /etc/init.d/oidentd start
sudo chkconfig oidentd on
```
