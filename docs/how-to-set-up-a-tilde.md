## Build your own tilde-style server (beginner-friendly, step-by-step)

This guide is intentionally detailed for first-time operators.

If you have never run a server before, follow this in order and do not skip ahead.

---

## What you are building

A "tilde-style" host usually provides:

- A Linux shell account for each user
- Personal web publishing from `~/public_html`
- Shared Unix tools for learning, writing, coding, and socializing
- Community norms and moderation

In short: a small, friendly, multi-user Unix community.

---

## 0) Before you touch a server

### 0.1 Buy or prepare these things first

1. A domain name (example: `domain.club`)
2. One Linux server (VPS is fine)
3. SSH client on your laptop
4. A text editor you can use comfortably

### 0.2 Keep this safety rule in mind

Never close your current SSH session until you have confirmed a **new** SSH session works with your latest config changes.

This one habit prevents most accidental lockouts.

---

## 1) Provision your Linux server

You can use AWS, Hetzner, Linode, DigitalOcean, or any other provider.

### Recommended minimum for a small starter community

- 2 vCPU
- 4 GB RAM
- 40 GB SSD
- Ubuntu LTS or Debian stable (easiest for beginners)

### 1.1 Point DNS at your server

At your DNS provider:

- Create an `A` record for `domain.club` -> your server IPv4
- Optionally create `AAAA` for IPv6

DNS can take time to propagate.

### 1.2 First login

From your local machine:

```bash
ssh root@domain.club
```

If your provider uses a default admin user (for example `ubuntu`), use that user and `sudo`.

---

## 2) Base system setup (packages, hostname, firewall)

This section includes both Debian/Ubuntu and Red Hat-family commands.

### 2.1 Update the system

**Debian / Ubuntu**

```bash
apt update
apt -y upgrade
```

**RHEL / Rocky / Alma / Fedora**

```bash
dnf -y upgrade
```

### 2.2 Install baseline tools

**Debian / Ubuntu**

```bash
apt -y install sudo git curl wget rsync tmux htop vim nano tree jq ufw
```

**RHEL / Rocky / Alma / Fedora**

```bash
dnf -y install sudo git curl wget rsync tmux htop vim nano tree jq
```

### 2.3 Set hostname

```bash
hostnamectl set-hostname domain.club
```

Check:

```bash
hostnamectl
```

### 2.4 Configure a basic firewall

If using `ufw` (common on Ubuntu):

```bash
ufw default deny incoming
ufw default allow outgoing
ufw allow 22/tcp
ufw allow 80/tcp
ufw allow 443/tcp
ufw enable
ufw status verbose
```

If using `firewalld` (common on RHEL-family):

```bash
systemctl enable --now firewalld
firewall-cmd --permanent --add-service=ssh
firewall-cmd --permanent --add-service=http
firewall-cmd --permanent --add-service=https
firewall-cmd --reload
firewall-cmd --list-services
```

---

## 3) Create a non-root admin account

Running daily admin tasks as `root` is risky.

### 3.1 Create admin user

```bash
adduser admin
```

On some distros:

```bash
useradd -m -s /bin/bash admin
passwd admin
```

### 3.2 Give sudo privileges

**Debian / Ubuntu**

```bash
usermod -aG sudo admin
```

**RHEL-family**

```bash
usermod -aG wheel admin
```

### 3.3 Test sudo

```bash
su - admin
sudo whoami
```

Expected output: `root`

---

## 4) SSH hardening (safe order, no lockouts)

Do this carefully.

### 4.1 Set up admin SSH key directory and file

```bash
install -d -m 700 /home/admin/.ssh
install -m 600 /dev/null /home/admin/.ssh/authorized_keys
chown -R admin:admin /home/admin/.ssh
```

### 4.2 Add your public key

On your local machine, show your public key:

```bash
cat ~/.ssh/id_ed25519.pub
```

Copy that line and paste it into:

`/home/admin/.ssh/authorized_keys`

Example on server:

```bash
printf '%s\n' 'ssh-ed25519 AAAA... your-key-comment' >> /home/admin/.ssh/authorized_keys
chown admin:admin /home/admin/.ssh/authorized_keys
chmod 600 /home/admin/.ssh/authorized_keys
```

### 4.3 Keep passwords ON while you test keys

Edit `/etc/ssh/sshd_config`:

```text
PermitRootLogin no
PubkeyAuthentication yes
PasswordAuthentication yes
ChallengeResponseAuthentication no
UsePAM yes
```

Reload:

```bash
systemctl reload sshd
```

From a **second local terminal**, test login:

```bash
ssh admin@domain.club
```

If this fails, fix it now. Do **not** continue.

### 4.4 Disable password auth only after key login works

Edit `/etc/ssh/sshd_config`:

```text
PasswordAuthentication no
```

Reload and test again from a second terminal:

```bash
systemctl reload sshd
ssh admin@domain.club
```

Only after success should you end your old session.

---

## 5) Install and configure Apache for user pages

This is the baseline many tilde hosts use for `~username` pages.

### 5.1 Install Apache

**Debian / Ubuntu**

```bash
apt -y install apache2
systemctl enable --now apache2
```

**RHEL-family**

```bash
dnf -y install httpd
systemctl enable --now httpd
```

Check service status:

```bash
systemctl status apache2 --no-pager
# or
systemctl status httpd --no-pager
```

### 5.2 Enable user directories (`~username` URLs)

#### Debian / Ubuntu

```bash
a2enmod userdir
systemctl restart apache2
```

By default, this serves `/home/USERNAME/public_html` as:

`http://domain.club/~USERNAME/`

#### RHEL-family

Edit Apache config (often `/etc/httpd/conf/httpd.conf`) and ensure:

```apache
UserDir public_html

<Directory /home/*/public_html>
    AllowOverride All
    Options MultiViews Indexes SymLinksIfOwnerMatch IncludesNoExec
    Require method GET POST OPTIONS
</Directory>
```

Then restart:

```bash
systemctl restart httpd
```

### 5.3 Create a test web user and page

```bash
useradd -m -s /bin/bash testuser
mkdir -p /home/testuser/public_html
cat > /home/testuser/public_html/index.html <<'HTML'
<!doctype html>
<html>
  <head><meta charset="utf-8"><title>testuser page</title></head>
  <body><h1>hello from testuser</h1></body>
</html>
HTML
chown -R testuser:testuser /home/testuser
chmod 755 /home/testuser
chmod 755 /home/testuser/public_html
chmod 644 /home/testuser/public_html/index.html
```

Now test locally on the server:

```bash
curl -I http://localhost/~testuser/
curl http://localhost/~testuser/
```

Then test from your laptop:

```bash
curl -I http://domain.club/~testuser/
```

If it fails, check Apache logs:

**Debian / Ubuntu**

```bash
tail -n 100 /var/log/apache2/error.log
```

**RHEL-family**

```bash
tail -n 100 /var/log/httpd/error_log
```

---

## 6) Prepare `/etc/skel` before inviting users

`/etc/skel` is copied into every new account. Set it up early.

### 6.1 Minimum recommended contents

- `.bashrc` and/or `.zshrc` with helpful comments
- `public_html/index.html` starter page
- A README with first commands and local rules
- Optional `public_gemini/` and `public_gopher/`

### 6.2 Example starter files

```bash
install -d -m 755 /etc/skel/public_html
cat > /etc/skel/public_html/index.html <<'HTML'
<!doctype html>
<html>
  <head><meta charset="utf-8"><title>Welcome</title></head>
  <body>
    <h1>It works!</h1>
    <p>Edit this file to publish your page.</p>
  </body>
</html>
HTML
```

```bash
cat > /etc/skel/README-FIRST.txt <<'TXT'
Welcome to the server.

Useful first commands:
- pwd
- ls -la
- nano ~/public_html/index.html

Your web page lives at:
http://domain.club/~YOURUSERNAME/
TXT
```

See also: `docs/etc-skel-permissions.md`.

---

## 7) Install baseline user tools

Give new users a capable, friendly default toolbox.

**Debian / Ubuntu**

```bash
apt -y install \
  zsh fish \
  emacs vim nano \
  irssi weechat \
  mutt alpine \
  lynx w3m links \
  git build-essential python3 nodejs npm
```

**RHEL-family**

```bash
dnf -y install \
  zsh fish \
  emacs vim nano \
  irssi weechat \
  mutt alpine \
  lynx links \
  git gcc make python3 nodejs npm
```

Add or remove packages based on your community.

---

## 8) Add users safely and consistently

Use a repeatable checklist every time.

### 8.1 Account creation checklist

```bash
useradd -m -s /bin/bash USERNAME
passwd -l USERNAME
install -d -m 700 /home/USERNAME/.ssh
install -m 600 /dev/null /home/USERNAME/.ssh/authorized_keys
install -d -m 755 /home/USERNAME/public_html
chown -R USERNAME:USERNAME /home/USERNAME
```

### 8.2 Add the user's public key

```bash
printf '%s\n' 'ssh-ed25519 AAAA... user@device' >> /home/USERNAME/.ssh/authorized_keys
chown USERNAME:USERNAME /home/USERNAME/.ssh/authorized_keys
chmod 600 /home/USERNAME/.ssh/authorized_keys
```

### 8.3 Verify login and web publishing

```bash
ssh USERNAME@domain.club
curl -I http://domain.club/~USERNAME/
```

---

## 9) Add "tilde functionality" in manageable layers

Do not launch everything on day one.

### Layer A: personal publishing

- User web pages in `public_html`
- Basic HTML templates
- Optional Gemini and Gopher directories

### Layer B: communication

- IRC client docs
- Local mail (postfix + local delivery)
- Server bulletin/MOTD updates

### Layer C: collaboration

- Shared Git repos
- Local pastebin/snippet service
- Community docs/wiki process

### Layer D: culture and learning

- New-user orientation checklist
- Mentoring or office-hours in chat
- Monthly "show your tilde" events

---

## 10) Operations, backup, and recovery

### 10.1 Back up the important data

At minimum, back up:

- `/home`
- `/etc`
- Web server config (`/etc/apache2` or `/etc/httpd`)
- Mail config if used

### 10.2 Example nightly backup script

Create `/usr/local/sbin/backup-tilde.sh`:

```bash
#!/usr/bin/env bash
set -euo pipefail

DEST=/var/backups/tilde
DATE=$(date +%F)
mkdir -p "$DEST/$DATE"

tar -czf "$DEST/$DATE/home.tgz" /home
tar -czf "$DEST/$DATE/etc.tgz" /etc
```

Make executable:

```bash
chmod 700 /usr/local/sbin/backup-tilde.sh
```

Run once manually:

```bash
/usr/local/sbin/backup-tilde.sh
```

Then automate with cron or systemd timers.

### 10.3 Test restore (critical)

A backup is not real until you test restoring at least one file.

---

## 11) First-week launch checklist

- [ ] DNS points to server
- [ ] Firewall allows only intended ports
- [ ] Admin key login works from a second terminal
- [ ] Password auth disabled only after key validation
- [ ] Apache running and `~testuser` page reachable
- [ ] `/etc/skel` tested by creating a new account
- [ ] Backup job ran and one restore test passed
- [ ] Rules/moderation/contact info published
- [ ] At least one backup admin has emergency access

---

## 12) Where to continue in this repository

- `docs/shellserver.md` for shell host operational notes
- `docs/etc-skel-permissions.md` for current skeleton permissions
- `docs/ssh.md` for SSH key onboarding details
- `docs/server.org` for historical package/setup notes
