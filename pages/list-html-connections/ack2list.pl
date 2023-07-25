#!/usr/bin/perl -w
use strict;
my %h;
while (<STDIN>) {
    chomp;
    my ($file, $line, $tilde) = split /:/;
    $tilde =~ s/\///;
    $file =~ s/\/home\//\/~/;
    $file =~ s/public_html\///;
    $h{$tilde}{$file}++;
}

print qq{<html><head><title>Tilde.Club Connections</title>
<link rel="stylesheet" type="text/css" href="css/social.css">
</head>
<body>
<h1>Tilde.club connections</h1>
};

print "<table>";
for my $x (sort keys %h) {
    print "<tr><td class=\"user\"><a href=\"/$x\">$x</a></td><td class=\"linkedby\">is linked by</td><td class=\"linkers\">";
    for my $y (sort keys %{$h{$x}}) {
        my $user = $y;
        if ($user =~ /~(\w+)/) {
            my $username = "~$1"; # Add the tilde (~) before the username
            print "<a href=\"/$username\">$username</a> ";
        }
    }
    print "</td></tr>\n";
}
print "</table>";
print qq{</body></html>};
