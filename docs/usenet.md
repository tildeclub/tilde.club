# Usenet in 60 Seconds

## Installation

Create Linux machine (This is based on Fedora 32)

    $ sudo -i
    root $ dnf update


    root $ dnf install innd

    root $ vim /etc/hostname # set your hostname to something like `news`
    root $ vim /etc/hosts # change local.localdomain to the new hostname

    # should probably restart the machine to get the new hostname

    root $ sudo -u news bash
    news $ vim etc/inn.conf

The top of my `inn.conf` looks like:
        
        mta:                         "/usr/sbin/sendmail -oi -oem %s"
        organization:                "tilde.club"
        ovmethod:                    tradindexed
        hismethod:                   hisv6
        pathhost:                    news.tilde.club
        pathnews:                    /usr/local/news

        #runasuser:
        #runasgroup:

        # General Settings

        domain:                      news.tilde.club
        #innflags:
        mailcmd:                     /usr/libexec/news/innmail
        server:                      news.tilde.club
        #syntaxchecks:               [ no-laxmid ]


Then you need to configure `readers.conf` to say who can connect, how, and what
kind of things they get when they log in.

    news $ mv /etc/news/readers.conf /etc/news/readers.conf.old
    news $ vim /etc/readers.conf

A rudimentary `readers.conf` looks like:

    # Since tilde.club is very open and inviting, I don't mind having
    # such a wild rule.
    
    auth tilde.club {
            hosts: *
            default: <PUBLIC>
    }

    access default {
            users: <PUBLIC>
            newsgroups: "*,!admin.*,!control,!control.*,!mode,!junk"
    }

    access admin {
            users: "artem,ftrain@tilde.club"
            newsgroups: *
            access: "RPA"
            perlfilter: false
    }

And finally set up the cron job to expire messages. Make sure you add this as
the `news` user cron:

SHELL=/bin/sh
MAILTO=root
#=========================================================================
# INN crontab:
#=========================================================================
#
# Run news.daily every morning at 6am
#
0 6 * * *               /usr/libexec/news/news.daily > dev/null


## Running the server

This can be accomplished via systemd. 

Here is how to enable it to start automatically:

    root $ systemctl enable innd

The following runs the server:

    root $ systemctl start innd

And the following stops it:

    root $ systemctl stop innd

All errors are logged to `/var/log/news`. `/var/log/errlog` is a good resource for finding
out why your news server isn't running. 



## Troubleshooting the server

If you know there are articles on the server and for some reason they do not show when you connect with a client, you can rebuild your history and index's.

    root $ systemctl stop innd
    
    root $ sudo -u news bash
        
    news $ cd /var/lib/news/
    
    news $ time /usr/libexec/news/makehistory -b -O -f history.n
    
    news $ awk 'NF == 2 { print }' < history >> history.n
    
    news $ /usr/libexec/news/makedbz -s `wc -l < history` -f history.n
    
    news $ mv  history.n history
    
    news $ mv history.n.dir  history.dir
    
    news $ mv history.n.hash history.hash
    
    news $ mv history.n.index history.index
    
    news $ exit
    
    root $ systemctl innd start
    
    root $ sudo -u news /usr/libexec/news/ctlinnd renumber ''
        
    Above guide was copied/modified from: https://lists.isc.org/pipermail/inn-workers/2003-September/012007.html
    
    
    
    
