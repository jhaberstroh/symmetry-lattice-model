#!/usr/bin/perl
# file: txt2csv.pl

$fname = shift;

open FILE, "<$fname" or die $!;
$csvname =~ 

$line = chomp(<FILE>);
print "$line";
close(<FILE>);
