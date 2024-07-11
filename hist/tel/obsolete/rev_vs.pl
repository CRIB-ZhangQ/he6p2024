#!/usr/bin/perl -n

if (/title/) {
#    print $_;
    $_ =~ /([\W\s]+)title: ([\w\s]+)vs([\w\s]+)/ ;
    if ($3) {
	$swapped = "$1title: $3 vs $2\n";
	$swapped =~ s/\n//;
	print "$swapped";
    }
    $_ =~ /([\W\s]+)name: ([\w\s]+)vs([\w\s]+)/ ;
    if ($3) {
	$swapped = "$1name: $3 vs $2\n";
	$swapped =~ s/\n//;
	print "$swapped";
    }

}
else {
    print $_;
}

