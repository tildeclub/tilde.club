ack --html "\/~\w+" /home -o 2>/dev/null|grep -v feed|grep -v anthonydpaul |perl ack2list.pl > public_html/social.html
