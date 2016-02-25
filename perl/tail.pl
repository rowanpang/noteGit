#!/usr/bin/perl

use File::Tail;
use Date::Calc qw(:all);

$x=7;

if ($x==7){
	print "true\n";
}

$this_year = This_Year();
print "$this_year ";
if (leap_year(This_Year()))
#if (leap_year(2000))
{
	print "leap year";
}
else
{
	print "not leap year";
}
print "\n";

$file=File::Tail->new(name=>"/home/pangwz/test/args/main.c",tail=>10,maxinterval=>2);
while (defined($line=$file->read)){
	print "$line";
}

print "end \n";
	
