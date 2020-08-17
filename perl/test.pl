#!/usr/bin/env perl
#

@var9=(1,2,3,4,5);
$var10=$var9[1];
$length=@var9;

print "$length \n";



@acc=map("%r$_",(8..11, "ax", "si", "bx", "bp", 12));
$len = @acc;
print "$len\n";
print "@acc\n";
