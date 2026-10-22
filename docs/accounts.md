# Accounts and first login

These notes describe the scripts collected from tilde.club on October 22, 2026.
They explain the dependencies and side effects that a replacement installation
must account for. Do not run the production scripts on a new machine before
changing the domains, paths, and external service integrations.

## What account creation currently does

`/usr/local/bin/makeuser` accepts a username, contact email, and SSH public key.
For a new username, it performs these operations in order:

1. Generate a 20-character password with `pwgen`.
2. Create a matching primary group if needed.
3. Create a Bash account and home, adding supplementary group ID 100.
4. Set the account password with `chpasswd`.
5. Render `/usr/local/bin/welcome-email.tmpl` and send it through `sendmail`.
   The message is also copied to the new local user and the root mailbox.
6. Write the supplied public key to the new home's `.ssh/authorized_keys`.
7. Announce the account through the local `toot` command.
8. Remove matching entries from `/var/signups_current`.
9. Remove the skeleton's `.git` directory and README from the new home.
10. Sort the password database with `pwck -s`.
11. Request an XFS block quota of 1 GiB soft and 3 GiB hard.
12. Create a ZNC account with the generated password and a three-network limit.
13. Add the contact address to the local helpdesk.

The mailing-list subscription function is present but its call is commented
out. PHP pool creation is not called by this copy of `makeuser`.

If the username already exists, the script exits successfully without doing
the remaining work. That is important after an interrupted run: invoking it
again does not complete a partially provisioned account. Check each dependent
service before treating that account as ready.

## Dependencies to resolve on another host

The common group must exist before account creation. Group ID 100 is a local
choice; check its meaning on the target instead of assuming that ID names the
same group everywhere.

The skeleton must supply `.ssh` with mode 0700 and `authorized_keys` with mode
0600. The script writes into that file; it does not create the parent directory
or independently repair its ownership and permissions.

The production script also needs Python 3, `pwgen`, working outbound mail, its
welcome template, XFS quota support, and these local integrations:

- `/usr/local/bin/toot`
- `/usr/local/bin/znccreate.py`
- `/usr/local/bin/helpdesk_admin.sh`

These are separate components. Merely copying `makeuser` does not install them.
The production command changes external services as well as the Unix account,
so an account test must use a test configuration for those services too.

## First-login wizard

`/usr/local/bin/firstlogin` is a Ruby program using `tty-prompt` and `tty-screen`.
It also uses `figlet`, `passwd`, `chsh`, `/etc/shells`, `/etc/editors`, `tzselect`,
and optionally Byobu. Its sequence is:

1. Change the account password using the temporary password from the welcome
   message.
2. Choose a login shell from an allowlist of names in `/etc/shells`.
3. Choose an editor and append `EDITOR` to `.bash_profile`.
4. Offer to enable Byobu on login.
5. Choose a timezone; write it to `.zshrc` for Zsh or `.bash_profile` otherwise.
6. Offer mail forwarding through `.forward`.
7. Offer to run `setup-2fa`.
8. Save pronouns in `.pronouns`.
9. Remove `.new_user`; launch Byobu if selected.

The wizard makes enrollment optional. The collected `setup-2fa` simply invokes
Google Authenticator, and the main SSH config accepts key-only login separately
from its PAM password/TOTP path. See [SSH and MFA](ssh.md) for the observed policy
and an explicit new-host enforcement example. The `.new_user` marker and shell
startup hook do not prevent someone from using another login path.

The collected wizard writes the editor setting to `.bash_profile` regardless
of the selected shell, and handles timezone setup specially only for Zsh. Test
those settings after selecting Fish or another shell before documenting them
as working for every choice.

## Checks for a newly created account

On a test installation, verify all of the following before sending an account
ready message:

- Primary and supplementary groups match the intended policy.
- The home, `.ssh`, and key file have the intended owner and permissions.
- A supplied key can log in, and an unrelated key cannot.
- First-login setup runs from each supported login path.
- Enrollment and subsequent SSH authentication behave as documented.
- The user's starter page works at the published URL.
- Mail delivery and any forwarding work.
- Quotas are enforced on the filesystem containing the home.
- ZNC and helpdesk provisioning completed, if those integrations are enabled.

Use a separate connection for login tests. Keep the existing admin connection
and provider console available while changing authentication.
