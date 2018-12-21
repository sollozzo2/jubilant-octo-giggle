#! /usr/bin/perl

my($fieldname, $directory) = split(/=/, $ENV{QUERY_STRING});
system ("./my-histogram $directory | gnuplot");

system ("perl formatpage.cgi");
