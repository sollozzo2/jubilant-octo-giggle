#! /usr/bin/perl

my($fieldname, $directory) = split(/=/, $ENV{QUERY_STRING});
system ("./my-histogram $directory | gnuplot");
if (-d $directory) {
    system ("perl formatpage.cgi");
} else {
	print "Content-type: text/plain\r\n\r\n";
    print "Invalid directory"
}
