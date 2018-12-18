#! /usr/bin/perl

my($fieldname, $directory) = split(/=/, $ENV{QUERY_STRING});
system ("./my-histogram $directory | gnuplot");


print "Content-type: text/html\r\n\r\n";
print "<HTML>\n";
print "<HEAD>\n";
print "<TITLE> File Histogram </TITLE>";
print "</HEAD>\n";
print "<BODY BGCOLOR=\"FFFFFF\">";
print "<CENTER><H1><font size=\"16\" color=\"red\">CS410 Webserver</font></H1></HR></CENTER>";
print "<HR>\n";
print "<CENTER><IMG SRC=\"myhistresult.gif\" ALIGN=\"BOTTOM\"> </CENTER>";
print "</BODY>";
print "</HTML>";
