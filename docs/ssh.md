[http://tilde.club](tilde.club) uses `ssh` public-key authentication.

If you want access to [tilde.club](http://tilde.club), create a new key pair with an encrypted key and send the **public** key to the admins.

##Mac

1. Open **Terminal** (in /Applications/Utilities).
1. In the **Terminal** window, paste the following:
<pre>
mkdir -p ~/.ssh                 # Create your .ssh directory
ssh-keygen -f ~/.ssh/tilde.club # Create your keys
</pre>
1. The `ssh-keygen` program will generate two new keys (private and public), and prompt you for a password. Please set a good password (at least 9 characters, and not just letters or numbers). Make a note of it somewhere safe.
1. `open ~/.ssh # Open your .ssh directory`
1. Email ford@ (our friendly host) and attach `tilde.club.pub` to the email (it should be visible in the Finder. **Do not** attach `tilde.club` -- that is your *private* key, which you should never share.

##Linux

If you're on Linux you can use the same commands to generate a `tilde.club` keypair, but attaching `tilde.club.pub` is left as an exercise for the reader.

##Windows

We need instructions for using `PuTTYgen` to create a key in OpenSSH compatible format.

##Why?

`ssh` keys are more secure than classic UNIX passwords. Someone who takes over the server (`sshd` program) can capture the UNIX passwords of everyone who uses them to login -- [this has happened](http://www.apache.org/info/20010519-hack.html).

Additionally, if you use an `ssh` agent, you can login without entering a password every time. On OS X the Apple Keychain provides this functionality. On Linux it's `ssh-agent` and possibly [keychain](http://www.funtoo.org/Keychain). On Windows `pageant` provides this capability for [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html).
