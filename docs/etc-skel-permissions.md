The [tilde.etcskel repository](https://github.com/tildeclub/tilde.etcskel) contains the contents of the `/etc/skel/` directory, but since git repositories can't reproduce permissions on the files, here are the current permissions for those contents:

```
root@tilde /e/skel (master)# ls -alR
.:
total 52
drwxr-xr-x.   9 root root   258 Aug  2 17:22 ./
drwxr-xr-x. 132 root root 12288 Sep 21 06:08 ../
-rw-r--r--.   1 root root    29 Sep 15  2019 .bash_logout
-rw-r--r--.   1 root root  3318 Mar 27 03:52 .bashrc
drwxr-xr-x    3 root root   303 Oct  8  2019 .byobu/
-rw-r--r--.   1 root root   500 Sep 15  2019 .emacs
drwxr-xr-x.   8 root root   185 Mar 27 06:19 .git/
drwxr-xr-x.   2 root root    35 Sep 20  2019 .irssi/
-rw-r--r--.   1 root root    11 Sep 15  2019 .logout
-rw-r--r--.   1 root root   124 May 17 02:01 .mkshrc
-rw-r--r--    1 root root     0 Mar 27 01:48 .new_user
-rw-r--r--    1 root root   816 Mar 27 03:05 .profile
drwxr-xr-x.   2 root root    23 Sep 15  2019 public_gopher/
drwxr-xr-x.   2 root root    24 Jul  7 03:43 public_html/
-rw-r--r--.   1 root root    12 Sep 15  2019 .scrollzrc
drwx------.   2 root root    29 Sep 15  2019 .ssh/
drwxr-xr-x.  11 root root  4096 Dec 16  2019 .weechat/
-rw-r--r--.   1 root root   658 Feb 24  2020 .zshrc

./.byobu:
total 52
drwxr-xr-x  3 root root  303 Oct  8  2019 ./
drwxr-xr-x. 9 root root  258 Aug  2 17:22 ../
-rw-r--r--  1 root root   19 Aug 23  2018 backend
drwxr-xr-x  2 root root    6 Aug 23  2018 bin/
-rw-r--r--  1 root root   38 Aug 23  2018 color
-rw-r--r--  1 root root   96 Aug 23  2018 color.tmux
-rw-r--r--  1 root root   45 Aug 23  2018 datetime.tmux
-rw-r--r--  1 root root   52 Aug 23  2018 keybindings
-rw-r--r--  1 root root   73 Aug 23  2018 keybindings.tmux
-rw-r--r--  1 root root    0 Aug 23  2018 .metadata_available
-rw-r--r--  1 root root   49 Aug 23  2018 profile
-rw-r--r--  1 root root   47 Aug 23  2018 profile.tmux
-rw-r--r--  1 root root   95 Aug 24  2018 prompt
-rw-r--r--  1 root root    0 Aug 23  2018 .screenrc
-rw-r--r--  1 root root 2833 Aug 23  2018 status
-rw-r--r--  1 root root 2558 Oct  8  2019 statusrc
-rw-r--r--  1 root root  114 Mar 27 04:00 .tmux.conf
-rw-r--r--  1 root root    0 Aug 23  2018 windows
-rw-r--r--  1 root root    8 Aug 23  2018 windows.tmux

./.byobu/bin:
total 0
drwxr-xr-x 2 root root   6 Aug 23  2018 ./
drwxr-xr-x 3 root root 303 Oct  8  2019 ../

./.irssi:
total 12
drwxr-xr-x. 2 root root   35 Sep 20  2019 ./
drwxr-xr-x. 9 root root  258 Aug  2 17:22 ../
-rw-r--r--. 1 root root 4971 Sep 20  2019 config
-rw-r--r--. 1 root root   34 Sep 15  2019 startup

./public_gopher:
total 4
drwxr-xr-x. 2 root root  23 Sep 15  2019 ./
drwxr-xr-x. 9 root root 258 Aug  2 17:22 ../
-rw-r--r--. 1 root root 153 Sep 15  2019 gophermap

./public_html:
total 4
drwxr-xr-x. 2 root root   24 Jul  7 03:43 ./
drwxr-xr-x. 9 root root  258 Aug  2 17:22 ../
-rw-r--r--  1 root root 1223 Jul  7 03:43 index.html

./.ssh:
total 0
drwx------. 2 root root  29 Sep 15  2019 ./
drwxr-xr-x. 9 root root 258 Aug  2 17:22 ../
-rw-------. 1 root root   0 Sep 15  2019 authorized_keys

./.weechat:
total 120
drwxr-xr-x. 11 root root  4096 Dec 16  2019 ./
drwxr-xr-x.  9 root root   258 Aug  2 17:22 ../
-rw-------.  1 root root   872 Sep 16  2019 alias.conf
-rw-------.  1 root root  1128 Sep 16  2019 buflist.conf
-rw-------.  1 root root   315 Sep 16  2019 charset.conf
-rw-------.  1 root root   378 Sep 16  2019 exec.conf
-rw-------.  1 root root   293 Sep 16  2019 fifo.conf
-rw-------.  1 root root  2313 Sep 16  2019 fset.conf
drwxr-xr-x.  3 root root    22 Sep 16  2019 guile/
-rw-------.  1 root root   299 Sep 16  2019 guile.conf
-rw-------   1 root root  4370 Dec 16  2019 irc.conf
-rw-------.  1 root root   598 Sep 16  2019 logger.conf
drwx------.  2 root root    37 Sep 16  2019 logs/
drwxr-xr-x.  3 root root    22 Sep 16  2019 lua/
-rw-------.  1 root root   297 Sep 16  2019 lua.conf
drwxr-xr-x.  3 root root    22 Sep 16  2019 perl/
-rw-------.  1 root root   298 Sep 16  2019 perl.conf
-rw-------.  1 root root   265 Sep 16  2019 plugins.conf
drwxr-xr-x.  3 root root    22 Sep 16  2019 python/
-rw-------.  1 root root   300 Sep 16  2019 python.conf
-rw-------.  1 root root  1043 Sep 16  2019 relay.conf
drwxr-xr-x.  3 root root    22 Sep 16  2019 ruby/
-rw-------.  1 root root   298 Sep 16  2019 ruby.conf
drwxr-xr-x.  2 root root     6 Sep 16  2019 script/
-rw-------.  1 root root  1254 Sep 16  2019 script.conf
-rw-------.  1 root root   329 Sep 16  2019 sec.conf
-rw-------.  1 root root   653 Sep 16  2019 spell.conf
drwxr-xr-x.  3 root root    22 Sep 16  2019 tcl/
-rw-------.  1 root root   297 Sep 16  2019 tcl.conf
-rw-------.  1 root root  2363 Sep 16  2019 trigger.conf
-rw-------.  1 root root 20156 Sep 16  2019 weechat.conf
-rw-r--r--.  1 root root  5741 Sep 16  2019 weechat.log
drwx------.  2 root root     6 Sep 16  2019 xfer/
-rw-------.  1 root root   915 Sep 16  2019 xfer.conf
