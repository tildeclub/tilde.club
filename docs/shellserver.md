# Setting up the tilde.club shell server (user host)

We want to document the ins and outs of setting up the server so others who are interested can learn (and help!).

## System setup

For now, this is all [documented in a separate server-setup document](https://github.com/tildeclub/tilde.club/blob/master/docs/server.org); ultimately, I presume we'll consolidate it all here.

## /etc/skel directory

This is the directory that's used as the basis for all newly-created users' home directories, so it's good to get it right before adding new users to a shell server. We've created [a separate repo for the contents of the directory itself](https://github.com/tildeclub/tilde.etcskel), but since it's impossible to check in the appropriate file and directory permissions, [they're documented here](https://github.com/tildeclub/tilde.club/blob/master/docs/etc-skel-permissions.md).

## Email

The default MTA on CentOS is `postfix`. Our goal was to have a `localhost`-only mail service, which required that we configure `postfix` to listen only to `localhost`, and to bounce any email which local users try to send off-server. Both configuration changes are handled in `/etc/postfix/main.cf`.

* the `inet_interfaces` value should just be `localhost` (`inet_interfaces = localhost`)
* the `default_transport` parameter should be the bounce message we want (so add `default_transport = error: outside mail is not deliverable` to the bottom of the file)

### pine

Pine is sort of brain-dead about creating its `.addressbook` file in a user's home directory with `744` permissions; there doesn't appear to be an option to fix this. Instead, it's probably important to work around it before adding any new users:

1. Add `~/mail/` to `/etc/skel` with permissions `700` so that there's a user-accessible-only place for the file to live.
2. Create an `/etc/pine.conf` file that includes the config directive `address-book=mail/.addressbook` to put that file into this new home.

## identd

Users will connect from their shell account to an IRC server, so it is *very* handy to have an `identd` server. For us that just meant installing the standard CentOS `identd` server and configuring it to start automatically:

```
sudo yum install oidentd
sudo /etc/init.d/oidentd start
sudo chkconfig oidentd on
```
