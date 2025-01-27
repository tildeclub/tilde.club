The [tilde.etcskel repository](https://github.com/tildeclub/tilde.etcskel) contains the contents of the `/etc/skel/` directory, but since git repositories can't reproduce permissions on the files, here are the current permissions for those contents:

```
root@tilde /e/skel (master)# ls -alR
.:
total 63
drwxr-xr-x.  10 root root  4096 Jul 17  2024 ./
drwxr-xr-x. 164 root root 12288 Jan 27 19:40 ../
-rw-r--r--.   1 root root    29 Sep 15  2019 .bash_logout
-rw-r--r--    1 root root   144 Aug 12 00:00 .bash_profile
-rw-r--r--    1 root root  3272 Dec 13  2021 .bashrc
drwxr-xr-x    3 root root   303 Jan 27 19:43 .byobu/
lrwxrwxrwx    1 root root    25 Jul 17  2024 .dovecot.sieve -> sieve/rainloop.user.sieve
-rw-r--r--.   1 root root   500 Sep 15  2019 .emacs
drwxr-xr-x.   2 root root    35 Mar 27  2023 .irssi/
-rw-r--r--.   1 root root    11 Sep 15  2019 .logout
-rw-r--r--    1 root root   124 Jul 18  2024 .mkshrc
-rw-r--r--    1 root root     0 Mar 27  2020 .new_user
-rw-r--r--    1 root root   816 Mar 27  2020 .profile
drwxr-xr-x    2 root root    23 Nov 26  2021 public_gemini/
drwxr-xr-x.   2 root root    23 Jan 27 19:43 public_gopher/
drwxr-xr-x.   2 root root    24 Mar 27  2023 public_html/
-rw-r--r--    1 root root   790 Dec 23  2022 README.md
-rw-r--r--.   1 root root    12 Sep 15  2019 .scrollzrc
drwxr-xr-x    3 root root    44 Jul 17  2024 sieve/
drwx------.   2 root root    29 Sep 15  2019 .ssh/
-rw-r--r--    1 root root   299 Jul 20  2024 .zprofile
-rw-r--r--    1 root root   658 Jul 20  2024 .zshrc

./.byobu:
total 56
drwxr-xr-x   3 root root  303 Jan 27 19:43 ./
drwxr-xr-x. 10 root root 4096 Jul 17  2024 ../
-rw-r--r--   1 root root   19 Aug 23  2018 backend
drwxr-xr-x   2 root root    6 Aug 23  2018 bin/
-rw-r--r--   1 root root   38 Aug 23  2018 color
-rw-r--r--   1 root root   96 Aug 23  2018 color.tmux
-rw-r--r--   1 root root   45 Aug 23  2018 datetime.tmux
-rw-r--r--   1 root root   52 Aug 23  2018 keybindings
-rw-r--r--   1 root root   73 Aug 23  2018 keybindings.tmux
-rw-r--r--   1 root root    0 Aug 23  2018 .metadata_available
-rw-r--r--   1 root root   49 Aug 23  2018 profile
-rw-r--r--   1 root root   47 Aug 23  2018 profile.tmux
-rw-r--r--   1 root root   95 Aug 24  2018 prompt
-rw-r--r--   1 root root    0 Aug 23  2018 .screenrc
-rw-r--r--   1 root root 2833 Aug 23  2018 status
-rw-r--r--   1 root root 2558 Oct  8  2019 statusrc
-rw-r--r--   1 root root  103 Jan 27 19:43 .tmux.conf
-rw-r--r--   1 root root    0 Aug 23  2018 windows
-rw-r--r--   1 root root    8 Aug 23  2018 windows.tmux

./.byobu/bin:
total 0
drwxr-xr-x 2 root root   6 Aug 23  2018 ./
drwxr-xr-x 3 root root 303 Jan 27 19:43 ../

./.irssi:
total 16
drwxr-xr-x.  2 root root   35 Mar 27  2023 ./
drwxr-xr-x. 10 root root 4096 Jul 17  2024 ../
-rw-r--r--   1 root root 4977 Mar 27  2023 config
-rw-r--r--   1 root root   15 Mar 27  2023 startup

./public_gemini:
total 8
drwxr-xr-x   2 root root   23 Nov 26  2021 ./
drwxr-xr-x. 10 root root 4096 Jul 17  2024 ../
-rw-r--r--   1 root root   96 Nov 26  2021 index.gmi

./public_gopher:
total 8
drwxr-xr-x.  2 root root   23 Jan 27 19:43 ./
drwxr-xr-x. 10 root root 4096 Jul 17  2024 ../
-rw-r--r--   1 root root  149 Jan 27 19:43 gophermap

./public_html:
total 8
drwxr-xr-x.  2 root root   24 Mar 27  2023 ./
drwxr-xr-x. 10 root root 4096 Jul 17  2024 ../
-rw-r--r--   1 root root 2145 Mar 27  2023 index.html

./sieve:
total 4
drwxr-xr-x   3 root root   44 Jul 17  2024 ./
drwxr-xr-x. 10 root root 4096 Jul 17  2024 ../
-rw-------   1 root root    0 Jul 17  2024 rainloop.user.sieve
drwx------   2 root root    6 Jul 17  2024 tmp/

./sieve/tmp:
total 0
drwx------ 2 root root  6 Jul 17  2024 ./
drwxr-xr-x 3 root root 44 Jul 17  2024 ../

./.ssh:
total 4
drwx------.  2 root root   29 Sep 15  2019 ./
drwxr-xr-x. 10 root root 4096 Jul 17  2024 ../
-rw-------.  1 root root    0 Sep 15  2019 authorized_keys

