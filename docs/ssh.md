[tilde.club](http://tilde.club) uses `ssh` public-key authentication.

If you want access to [tilde.club](http://tilde.club), you'll need to create a new SSH key pair and then send the **public** key to the admins; they will then associate it with your account and let you know when things are ready for you to log in.

## Mac

### Generating your key pair

1. Open **Terminal** (in `/Applications/Utilities/`).
1. In the **Terminal** window, paste the following:
<pre>
mkdir -m 700 -p ~/.ssh                          # Create your .ssh directory
ssh-keygen -t rsa -b 2048 -f ~/.ssh/tilde.club  # Create your keys
</pre>
1. The `ssh-keygen` program will generate two new keys (private and public), and prompt you for a password. Please set a good password (at least 9 characters, and not just letters or numbers), and make a note of it somewhere safe.
1. Now, in **Terminal**, type: `open ~/.ssh` to open a Finder window showing your `.ssh` directory.
1. Take the `tilde.club.pub` file and attach it to an email to ford@ (our friendly host). **Do not attach the file `tilde.club`** -- that is your *private* key, which you should never ever **ever** share with anyone.

### Using your key pair

1. Open **Terminal**.
1. type `ssh -i ~/.ssh/tilde.club username@tilde.club` (substituting in your own username)

Note that it's possible to save an SSH configuration so that you don't have to type this whole long thing every time; [here's a good primer](http://nerderati.com/2011/03/17/simplify-your-life-with-an-ssh-config-file/) on how to do that.

## Linux

### Generating your key pair

If you're on Linux you can use the same commands as on the Mac to generate a `tilde.club` key pair, but attaching `tilde.club.pub` is left as an exercise for the reader.

### Using your key pair

At a command prompt, type `ssh -i ~/.ssh/tilde.club username@tilde.club` (substituting your own username).

Note that it's possible to save an SSH configuration so that you don't have to type this whole long thing every time; [here's a good primer](http://nerderati.com/2011/03/17/simplify-your-life-with-an-ssh-config-file/) on how to do that.

## Windows

### Generating your key pair

The easiest method to generate a key pair on Windows is to use [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/), a freely-available SSH client which comes with its own key generator.

1. Download PuTTY [here](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html). (Specifically, get the Windows Installer version; if you want to download the binaries individually, you want to make sure you get the PuTTYgen binary which is what generates key pairs.)
1. Run **PuTTYgen**.
1. From the main window, click the **Generate** button; you'll be asked to move the mouse around to generate randomness, which you should do until PuTTYgen tells you that it's generating the key.
1. Once the key pair is generated, click the **Save public key** button, choose a directory you want to save it into, and in the "File name" field, type `tilde.club.pub`. Save the key.
1. Halfway down the main window, type a good password into the "Key passphrase" field (at least 9 characters, and not just letters or numbers), and repeat it in the "Confirm passphrase" field.
1. Click the **Save private key** button, choose the same directory as you did above, type `tilde.club` into the "File name" field, and save the private key.
1. Finally, email the `tilde.club.pub` file to ford@ (our friendly host). **Do not attach the file `tilde.club`** -- that is your *private* key, which you should never ever **ever** share with anyone.

### Using your key pair

1. Run **PuTTY**.
1. In the first window that comes up, you should be in the main "Session" category (in the left-hand column). On the right, in the "Host Name" field, type `tilde.club`.
1. In the left-hand column, find the **SSH** entry underneath the **Connection** section and then click on the small plus sign next to it to open it up; then find the **Auth** entry beneath it and click on it.
1. On the right, the last field should be "Private key file for authentication"; click the **Browse** button and find the `tilde.club` file you saved above. Select it and click the **Open** button.
1. Finally, click the **Open** button at the bottom of the main PuTTY window to open your connection. You'll be asked for your username ("login as:"); type it in and hit Enter. You'll then be asked for your key passphrase; type it in and hit enter, and you're in.

Note that you can save these settings from that first page of the main PuTTY window, so you don't have to do this every time.

## Why go through all this?

One of the weakest links in online account security is your password. If someone were to take over the tilde server (specifically the SSH server, `sshd`), they would be able to capture the passwords of everyone who uses a password to log in. (This isn't theoretical -- [it has actually happened](http://www.apache.org/info/20010519-hack.html).) If users use their SSH keys to log in instead, hackers can't do this anymore; a user's private key never leaves their own computer and can't be captured on the server.

Additionally, if you use an `ssh` agent, you can login without entering a password every time. On OS X the Apple Keychain provides this functionality. On Linux it's `ssh-agent` and possibly [keychain](http://www.funtoo.org/Keychain). On Windows `pageant` provides this capability for [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/).
