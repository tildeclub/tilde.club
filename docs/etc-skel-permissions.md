The [tilde.etcskel repository](https://github.com/tildeclub/tilde.etcskel) contains the contents of the `/etc/skel/` directory, but since git repositories can't reproduce permissions on the files, here are the current permissions for those contents:

```
[root@tilde skel]# ls -alR
.:
total 40
drwxr-xr-x  5 root root 4096 Oct 10 20:36 .
drwxr-xr-x 90 root root 4096 Oct 15 02:39 ..
-rw-r--r--  1 root root   29 Oct 10 12:02 .bash_logout
-rw-r--r--  1 root root  176 Sep 26 00:25 .bash_profile
-rw-r--r--  1 root root  124 Sep 26 00:25 .bashrc
-rw-r--r--  1 root root  500 Sep  4  2013 .emacs
drwx------  2 root root 4096 Oct 10 14:23 .irssi
-rw-r--r--  1 root root   11 Oct 10 12:03 .logout
drwxr-xr-x  2 root root 4096 Oct 13 19:39 public_html
drwx------  2 root root 4096 Oct 10 20:36 .ssh

./.irssi:
total 20
drwx------ 2 root root 4096 Oct 10 14:23 .
drwxr-xr-x 5 root root 4096 Oct 10 20:36 ..
-rw-r----- 1 root root 4910 Oct 11 14:18 config
-rw-r--r-- 1 root root   12 Oct 10 14:23 startup

./public_html:
total 12
drwxr-xr-x 2 root root 4096 Oct 13 19:39 .
drwxr-xr-x 5 root root 4096 Oct 10 20:36 ..
-rw-r--r-- 1 root root  323 Oct 13 19:44 index.html

./.ssh:
total 8
drwx------ 2 root root 4096 Oct 10 20:36 .
drwxr-xr-x 5 root root 4096 Oct 10 20:36 ..
-rw-r--r-- 1 root root    0 Oct 10 20:36 authorized_keys
```
