# SSH access and MFA

## Member keys

Use an SSH client already supplied with Linux, macOS, or Windows OpenSSH. On
Windows, PowerShell can run the same `ssh` and `ssh-keygen` commands. PuTTY is
also fine if the user prefers it; submit the OpenSSH-format public-key line,
not a `.ppk` private key.

On your own computer, generate a separate key if you do not already have one
you want to use. Choose a new filename rather than overwriting an existing key:

```bash
ssh-keygen -t ed25519 -a 64 -f ~/.ssh/tilde_ed25519
```

Use a passphrase. Send only `tilde_ed25519.pub` to the operator or signup form.
The file without `.pub` is private and stays on your computer. A hardware-backed
key can also be used when both the client and host support it.

Connect with:

```bash
ssh -i ~/.ssh/tilde_ed25519 member@tilde.example.net
```

A local `~/.ssh/config` entry saves typing:

```sshconfig
Host mytilde
    HostName tilde.example.net
    User member
    IdentityFile ~/.ssh/tilde_ed25519
    IdentitiesOnly yes
    ForwardAgent no
```

Verify a new host's fingerprint through a separate trusted channel. If it
changes later, ask why before replacing the saved host key. Agent forwarding
is normally unnecessary for using the host; enable it only for a specific
reason you understand.

## What the collected host does

The main daemon reads `/etc/ssh/sshd_config`. It has public-key support,
`PasswordAuthentication no`, keyboard-interactive authentication enabled, and
`UsePAM yes`. `AuthenticationMethods` is not set to require a combination;
the observed effective value is `any`.

Its SSH PAM authentication stack requires `pam_unix` and
`pam_google_authenticator`, except for a special help account. That means the
PAM route asks for the Unix password and a TOTP code. It does **not** mean
public-key login must also complete that route: the collected SSH configuration
accepts a valid public key on its own. Existing account-specific exceptions and
a separate restricted help command must be reviewed locally rather than copied
into a new installation.

`setup-2fa` is a small wrapper around `/usr/bin/google-authenticator`.
`check_google_authenticator.sh` is not referenced by the collected SSH PAM file;
its printed output is not an enforced authentication policy.

The main config collected from this host does not contain an `Include` for
`sshd_config.d`. The presence of files in that directory does not make them
active. Check the actual service command and configuration before assuming a
drop-in changes anything.

## Key-only baseline for a new host

Use [the example drop-in](../examples/ssh/00-tilde.conf) only after admin key
login and sudo have been tested. Put it in a directory that the actual main
config includes at global scope. Fedora's default layout normally provides
`/etc/ssh/sshd_config.d/*.conf`; existing customized configs may not.

OpenSSH generally uses the first value it obtains for a setting. A file placed
later in the include order will not necessarily override an earlier value.
Keep `Match` sections at the end and finish a conditional section with
`Match all` before adding unrelated global settings.

Before applying anything, keep a backup of the main file, its includes, the
SSH PAM file, and any unit overrides. Keep a working connection and the provider
console available. Validate the **effective** settings as well as syntax:

```bash
sshd -t
sshd -T -C user=member,host=client.example.net,addr=192.0.2.20
systemctl reload sshd
```

Run connection tests from a second terminal on your own computer. Test the
admin and member separately. Match rules based on source address or local port
also need representative `addr`, `laddr`, and `lport` values in the test context.
A successful syntax check alone does not establish that the intended method
will be accepted.

## Require MFA after enrollment

For a new host, keep the key-only baseline working while enrolling people.
The following is an example policy, not a report of the production host's
current enforcement:

1. Install `google-authenticator` and ensure time synchronization is healthy.
2. Create a group named `mfa-ready`.
3. Have the test member run `google-authenticator` interactively in their own
   account. Use time-based tokens, save the recovery codes somewhere private
   off the host, and complete a code check. Keep `.google_authenticator` private.
4. Keep the Fedora SSH PAM account/session stack. Add
   `auth required pam_google_authenticator.so` to `/etc/pam.d/sshd` after its
   normal authentication rules. Do not use `nullok` for an account that must
   provide a second factor. Do not edit authselect-generated shared files to
   make this SSH-only change.
5. Add the following conditional block at the end of the effective SSH config,
   after the baseline global settings:

```sshconfig
Match Group mfa-ready
    KbdInteractiveAuthentication yes
    AuthenticationMethods publickey,keyboard-interactive:pam
Match all
```

With Fedora's password authentication PAM substack retained, this asks for a
key, the Unix password, and the authenticator code. If you want a key plus TOTP
without the Unix password, design and test that SSH-specific PAM stack instead
of removing password checks from the shared system authentication configuration.

Validate the config, then add only the enrolled test account to the group:

```bash
groupadd -f mfa-ready
usermod --append --groups mfa-ready member
sshd -t
sshd -T -C user=member,host=client.example.net,addr=192.0.2.20
systemctl reload sshd
```

From the client, test correct and incorrect codes and confirm that key-only
batch authentication fails for the enrolled account:

```bash
ssh -o BatchMode=yes -o PreferredAuthentications=publickey \
    -i ~/.ssh/tilde_ed25519 member@tilde.example.net
```

That last command is a negative test and should fail under the enforced policy.
Also verify normal interactive login, SFTP if offered, recovery codes, and admin
recovery. Scripts using SSH need a separately documented policy; silently
exempting all automation defeats a blanket MFA claim.

Only enroll more members after the full test passes. A user-editable startup
file or `.new_user` marker is onboarding state, not an access-control boundary.

## Legacy SSH

The existing host has a second daemon on TCP 2222. Its service reads
`/etc/ssh/sshd_config_legacy`, disables root and password login, restricts its
allowed user, and deliberately enables old RSA/SHA-1 and group1 algorithms.
This is a compatibility exception, not a general access method.

A new tilde does not need this service. If you have a specific legacy client
requirement, isolate it in a separate daemon and source/identity policy, verify
its effective settings with `sshd -T -f PATH`, and record who needs it and when
it can be retired. Do not weaken the normal daemon or the host-wide crypto
policy for every member.

## Recovery

Verify the person's identity before replacing a key or resetting MFA. Remove
lost factors, invalidate unused recovery codes when re-enrolling, and check
whether other services share the same account password. Test that the revoked
key or factor no longer works. Do not ask someone to send a private SSH key or
their TOTP secret in an ordinary support message.

References: [OpenSSH server configuration](https://man.openbsd.org/sshd_config)
and [Google Authenticator PAM](https://github.com/google/google-authenticator-libpam).
