# Usenet in 60 Seconds

## Installation

Create Linux machine

    $ sudo -i
    root # yum update


    root # yum install gcc
    root # yum install cpan
    root # yum install python-devel

    root # Get the Mime parser (for controlchan [idk wtf that is])
    root # cpan -f install Mime::Parser

    root # wget ftp://ftp.isc.org/isc/inn/inn-2.5.4.tar.gz
    root # tar xvf inn-2.5.4.tar.gz
    root # cd inn-2.5.4

    root # groupadd news
    root # useradd -g news -d /usr/local/news news

    root # ./configure \
    --with-python \
    --enable-libtool \
    --with-gnu-ld \
    --with-sendmail=/usr/sbin/sendmail \
    --enable-tagged-hash \
    --enable-shared \
    LIBS="-lpthread"

    root # vim /etc/hostname # set your hostname to something like `news`
    root # vim /etc/hosts # change local.localdomain to the new hostname

    # should probably restart the machine to get the new hostname

    root # su - news
    news $ vim etc/inn.conf

The top of my `inn.conf` looks like:

    mta:                         "/usr/sbin/sendmail -oi -oem %s"
    organization:                "tilde.club"
    ovmethod:                    tradindexed
    hismethod:                   hisv6
    pathhost:                    news
    pathnews:                    /usr/local/news

    #runasuser:                  # default: news
    #runasgroup:                 # default: news

    # General Settings

    domain:                      news.tilde.club
    #innflags:
    mailcmd:                     /usr/local/news/bin/innmail
    #server:


Then you need to configure `readers.conf` to say who can connect, how, and what
kind of things they get when they log in.

    news $ mv etc/readers.conf etc/readers.conf.old
    news $ vim etc/readers.conf

A rudimentary `readers.conf` looks like:

    # as this machine will only listen to connections from the tilde.club server, I don't mind having
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

Ensure that all the history files have the right permissions:

    news $ chmod 644 ~/db/*

And finally set up the cron job to expire messages. Make sure you run this as
the `news` user:

    0 3 * * * ~/bin/news.daily expireover lowmark

## Running the server

There are some scripts in the `contrib` which will help you get started, namely
`sample.init.script`. I think it goes into `/etc/init.d`, though I might be
mistaken. Another option is a long-running tmux session. Either way, the
following runs the server:

    news $ bin/rc.news start

And the following stops it:

    news $ bin/rc.news stop

All errors are logged to `~/log/`. `~/log/errlog` is a good resource for finding
out why your news server isn't running. 
