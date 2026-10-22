# Mail

Mail is part of the current tilde.club host, not a localhost-only experiment.
The collected Postfix config accepts mail for tilde.club and several related
domains, uses `.mail/`, and has no configured `relayhost`. Dovecot 2.4 serves
Maildir mail from `~/.mail`, authenticates through PAM, and exposes a Postfix
authentication socket. The first-login wizard can create `.forward`.

OpenDKIM and Rspamd are connected through milters on localhost ports 8891 and
11332. A pyspf-milter service is also running; its presence alone does not show
how it is connected. The guide does not reconstruct unpublished filter rules.

## Decide what service you are offering

There are three useful starting points:

- Local-only mail between shell accounts.
- Internet mail with direct inbound and outbound delivery.
- Inbound mail with outbound delivery through an authenticated relay.

The production reference matches the second configuration at the Postfix
`relayhost` setting. Provider-side routing was not inspected. For a new direct
mail host, verify that TCP 25 is permitted, the address has working reverse DNS,
and the provider allows mail service. Do not discover a port-25 block after
opening accounts.

Use a separate mail domain in examples if that is your intended policy. Here,
mail addresses are `member@tilde.example.net` and the SMTP host is also
`tilde.example.net`.

## DNS before launch

Publish and verify:

- A/AAAA records for the SMTP host; publish IPv6 only if SMTP and reverse DNS work there.
- An MX for the mail domain pointing to that hostname, not to an IP literal.
- A PTR for each sending address, matching the sending hostname with forward resolution.
- SPF authorizing the actual sending service, including any outbound relay.
- A DKIM public key for the signing selector you configure.
- DMARC with a monitored report address and a policy you can operate. Start with
  observation and verify alignment before enforcing rejection.

Do not copy tilde.club's IP addresses, signing keys, DNS selectors, or domain
aliases. Forwarded mail has additional SPF/DMARC consequences; test it against
actual recipients rather than assuming `.forward` solves deliverability.

## Install the base services

```bash
dnf install postfix dovecot dovecot-pigeonhole swaks
```

Back up existing configs first. The examples are for a new host. Merge
[main.cf.fragment](../examples/postfix/main.cf.fragment) into Postfix's config,
replacing conflicting settings rather than appending duplicate guesses. Keep
the distribution's default local-recipient checks, alias handling, and master
services. Point `root` and `postmaster` aliases to a mailbox an operator reads,
then run `newaliases`.

The example uses Dovecot LMTP for local mailbox delivery so the Sieve plugin can
run. This is a deliberate new-host choice: the collected production config has
an empty `mailbox_transport` and a local delivery transport. A configured Sieve
plugin alone does not prove that every live delivery goes through it.

For a local-only host, keep Postfix listening on loopback, do not publish an MX
or open SMTP externally, and explicitly reject nonlocal delivery with
`default_transport = error: external mail is not offered`. Do not apply that
setting to a host that promises Internet mail.

## Dovecot 2.4

Use [tilde.conf](../examples/dovecot/tilde.conf) as a standalone starting config
for system accounts with private primary groups. Replace the domain and TLS
paths. Do not include another conflicting authentication config alongside it.
Retain the distro's `/etc/pam.d/dovecot` authentication/account policy; it is a
separate PAM service from SSH and should not inherit SSH-specific TOTP prompts.

This uses Dovecot 2.4 syntax. In particular, it uses `mail_driver`, `mail_path`,
protocol/plugin blocks, and current TLS settings. Do not paste a Dovecot 2.3
`mail_location` tutorial into it.

The example enables IMAP and LMTP, requires TLS, uses a minimum of TLS 1.2, and
places LMTP and authentication sockets inside Postfix's private spool. The
current host also enables POP3 and ManageSieve; enable those on a new host only
if they are part of the service you intend to support.

The recorded host has `ssl_min_protocol = TLSv1`. That is a live compatibility
setting, not the minimum used by this example. Check client compatibility before
changing a running system.

Ensure each mailbox and Sieve directory is owned by its member and is not
readable by other members or the shared web group. Test SELinux access on a new
enforcing host. Inspect `doveadm user member` for the actual resolved UID, group,
home, and mail paths.

```bash
doveconf -n
postfix check
systemctl enable --now dovecot postfix
```

Check the journal and actual socket ownership. A parser check cannot prove
that PAM login or delivery works. Do not add `doveconf -P` when sharing output:
it reveals values that the default display hides.

## Submission

Merge the named services from
[master.cf.fragment](../examples/postfix/master.cf.fragment). Do not replace the
rest of `master.cf`. The example offers authenticated STARTTLS on 587 and
implicit TLS on 465, with authentication enabled on those services and TLS
required before accepting credentials. Delivery on 25 remains available without
mandatory TLS so remote mail servers can reach you.

Keep relay restrictions explicit. Test that an outside, unauthenticated client
cannot relay to a third-party domain. Also test that an authenticated member
can submit a message and that the expected envelope/header sender policy is
enforced. If you want one-account/one-sender enforcement, maintain
`smtpd_sender_login_maps` for the addresses and aliases you actually allow and
then apply the matching sender restrictions to submission. Do not enable a
sender-ownership rule against an empty map.

A shared host's localhost clients are not all trusted operators. Local shell
users can invoke mail tools directly, so address-spoofing controls, outbound
volume policy, abuse response, and logging need to cover local submission too.
Use authenticated submission or an identity-aware local submission policy where
account attribution is required.

## Filtering and signing

The base examples establish mail transport, not a finished spam policy. Before
opening Internet mail, configure maintained filtering and signing software.
The production host's two milter endpoints are recorded above, but its private
rules and signing material are not included here.

If using OpenDKIM, install the maintained package and generate a new key under
`/etc/opendkim/keys/tilde.example.net`. Configure a signing table for your domain,
a key table pointing to that key, and a localhost-only milter socket. Publish
the generated public TXT record. Keep the private key restricted to the signing
service. In Rspamd, configure the proxy/milter and controller according to its
installed version, keep the controller private, and choose which service owns
DKIM signing. Avoid accidentally signing twice with conflicting policies.

Add the tested milter endpoints to `smtpd_milters` and `non_smtpd_milters` only
after verifying them. Choose and document what happens if filtering is down:
accepting unfiltered mail and temporarily rejecting it are different policies.
Add spam-folder handling only after confirming messages travel through the
Sieve-enabled delivery path. Do not enable Sieve external-program extensions
just because they appear in a production dump.

See [OpenDKIM](https://www.opendkim.org/) and
[Rspamd's MTA integration guide](https://docs.rspamd.com/tutorials/integration/).
Both need their own update, key rotation, and failure checks.

## Verification

Use disposable mailboxes and clients you control:

1. Deliver locally between two accounts and verify the mailbox path and ownership.
2. Receive from an outside sender and send to an outside recipient.
3. Verify the received SPF, DKIM, and DMARC results and inspect the queue/logs.
4. Check IMAP login with a valid account, a wrong password, and an ineligible account.
5. Check submission over TLS and reject an unauthenticated external relay attempt.
6. Test aliases, `.forward`, invalid recipients, a full quota, Sieve, and service restart.
7. Renew the certificate in a test run and confirm each mail service uses the new one.

For TLS inspection without exposing a password in command history:

```bash
openssl s_client -connect tilde.example.net:993 -servername tilde.example.net
openssl s_client -starttls smtp -connect tilde.example.net:587 -servername tilde.example.net
postqueue -p
journalctl -u postfix -u dovecot --since today
```

Do not submit a password until the client verifies the certificate and expected
hostname. Do not paste authentication transcripts into public logs.

Back up mailbox data, Sieve files, aliases, transport/filter configs, and signing
keys using [the recovery plan](backups.md). A key or MFA reset for SSH does not
by itself revoke an existing webmail session, mail password, or application token.
