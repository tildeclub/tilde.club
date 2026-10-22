# Home skeleton and permissions

`useradd --create-home` copies `/etc/skel` into a new home. Get it right before
creating accounts. Changes to the skeleton do not update existing homes.

The reference source is [tilde.etcskel](https://github.com/tildeclub/tilde.etcskel).
Review its startup commands, domains, IRC settings, and first-login dependencies
before deploying it. Stage changes outside `/etc/skel`; do not clone over a live
skeleton or copy a repository's `.git` directory into every new account.

## Minimal new-host skeleton

Keep the distribution's shell files, then add the public page and an empty
SSH key file. On a new skeleton only:

```bash
install -d -o root -g root -m 0700 /etc/skel/.ssh
install -o root -g root -m 0600 /dev/null /etc/skel/.ssh/authorized_keys
install -d -o root -g root -m 0755 /etc/skel/public_html
```

The `install ... /dev/null` command empties its destination. Do not use it on
an existing account's key file. Add a starter `index.html` at mode 0644, for
example:

```html
<!doctype html>
<html lang="en">
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>My tilde page</title>
<h1>It works.</h1>
<p>Edit ~/public_html/index.html to make this page your own.</p>
</html>
```

Add a short README covering SSH, editing the page, mail if offered, chat,
acceptable use, quotas, and how to reach an operator. Use your own domain and
contact details. Keep private defaults out of publicly served directories.

## Modes

| Path | Mode | Owner in the skeleton | Purpose |
| --- | --- | --- | --- |
| `/etc/skel` | 0755 | root:root | Template directory. |
| `.ssh` | 0700 | root:root | Private SSH configuration directory. |
| `.ssh/authorized_keys` | 0600 | root:root | Empty placeholder; never a shared key. |
| `public_html` | 0755 | root:root | Public web directory. |
| `public_html/index.html` | 0644 | root:root | Public starter page. |
| `public_gopher`, `public_gemini` | 0755 | root:root | Only if those services are offered. |
| Public text and menu files | 0644 | root:root | Readable publishing content. |
| Private mail/sieve directories | 0700 where appropriate | root:root | Not web content. |
| Private sieve files | 0600 | root:root | User-specific filtering state. |

`useradd` assigns the copied files to the new account. Check that ownership
rather than recursively changing an established home. Git records executable
bits but not arbitrary file modes or ownership, so a checkout alone is not a
permissions deployment procedure.

The collected production skeleton also has Byobu, Irssi, `.gnupg`, Sieve files,
a `.dovecot.sieve` symlink, and a `.new_user` marker. Its `.ssh` and `.gnupg`
directories are 0700, the empty key file and personal Sieve file are 0600,
public directories are 0755, and public starter files are 0644. A symlink must
be checked at its destination as well; do not use recursive chmod across it.

## First-login hook

On the collected host, `.bash_profile` sources `.bashrc`, and interactive Bash
runs `firstlogin` when `.new_user` exists. Noninteractive Bash returns before
that hook. The collected Zsh startup files do not contain the same hook.

The Ruby wizard removes the marker at the end. Test interruption, failure, and
re-entry. Keep it out of noninteractive SCP/SFTP or command sessions, and do
not rely on the marker to enforce MFA or block an account.

## Test a fresh copy

Create a disposable account with the real account-creation method. Check:

```bash
namei -l /home/member/.ssh/authorized_keys
namei -l /home/member/public_html/index.html
stat -c '%a %U:%G %n' /home/member /home/member/.ssh \
    /home/member/.ssh/authorized_keys /home/member/public_html
```

Use the home traversal mode explained in [the setup guide](how-to-set-up-a-tilde.md).
Check SSH and web access as the actual member and service users, including
SELinux labels when enforcing. Never repair a 403 by making the entire home
readable to everyone.
