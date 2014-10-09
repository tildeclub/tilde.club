## Preamble

This document will talk you through setting up your own tilde.club flavoured server; the example `domain.club` is used below. 

## Prerequisites

* An [Amazon AWS account](https://aws.amazon.com), though any other cloud provider would work just as well.
* Your own domain
* A thirst for the unknown

## Create an instance on Amazon EC2

* On AWS click `Launch Instance`
* Select `Amazon Linux AMI`
* Select `t2.micro`
* Select the (recommended) thingy in the popup
* Click `Launch`
* In `Security Groups` right-click:
   * `Edit inbound rules` -> `Add Rule` -> `HTTP` and `SSH`
   * `Edit outbound rules` -> `Add Rule` -> `HTTP` and `HTTPS`
* In `Elastic IPs`:
   * Click `Allocate New Address`
   * Choose `VPC` on the dropdown (it won't work otherwise, I forget why)
   * Right click, `Associate Address`
   * Choose the instance you just created
* Create an `A record` at your domain name registrar to point at the Elastic IP then wait for those changes to propagate.

**NOTE** This could take up to 48 hours, use `dig domain.club +nostats +nocomments +nocmd` to see if you're in business.

## SSH into your shiny instance using your `pem` file

* `ssh -i security.pem ec2-user@domain.club`
* `yum update`
* `sudo yum remove java`
* `sudo yum install git`

## Change hostname

* `sudo vim /etc/hosts` change `localhost.localdomain` to `domain.club`
* `sudo vim /etc/sysconfig/network` change `localhost.localdomain` to `domain.club`
* `sudo reboot`

## Allow passwords to log in

* `sudo vim /etc/ssh/sshd_config` change `PasswordAuthentication` to `yes`
* `sudo service sshd restart`

## Create a test user `foo` account and `public_html` folder

* `sudo adduser foo`
* `sudo passwd foo`
* `sudo mkdir /home/foo/public_html`
* `sudo chown foo:foo /home/foo/public_html`
* `sudo chmod 755 /home/foo`
* `sudo chmod 755 /home/foo/public_html`

## Install Apache

* `sudo yum install httpd`
* `sudo /etc/init.d/httpd start`
* `sudo vim /etc/httpd/conf/httpd.conf`:
  * comment out `UserDir disabled`
  * uncomment `UserDir public_html`
  * uncomment the `Control access to UserDir directories` block beginning with `<Directory /home/*/public_html>`
* `sudo /etc/init.d/httpd restart`

## Install other software

* `yum install tmux`
* `yum install mutt`
* `yum install irssi`
* `yum install tree`
* `yum install lynx`
* `yum install finger`
* etc
