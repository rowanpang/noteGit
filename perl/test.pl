#!/usr/bin/env perl
#
#
#$flavour = shift;
#$output  = shift;
#if ($flavour =~ /\./) { $output = $flavour; undef $flavour; }

#$win64=0; $win64=1 if ($flavour =~ /[nm]asm|mingw64/ || $output =~ /\.asm$/);

$0 =~ m/(.*[\/\\])[^\/\\]+$/;
$file = $0;
$dir=$1;

print "$0\n";
print "$1\n";

@var9=(1,2,3,4,5);
$var10=$var9[0];

print "$var10\n";           #1
$length=@var9;
print "$length \n";         #5



@acc=map("%r$_",(8..11, "ax", "si", "bx", "bp", 12));
$len = @acc;
print "$len\n";             #9
print "registers: @acc\n";

sub sumlr {
    my $cnt;
    for $var (@_) {
        $cnt += $var;
    }

    return $cnt;
}

use feature 'signatures';                       #
sub sumlr2($left,$right) {                      #需要声明 signature 特性, ref https://perldoc.perl.org/perlsub#Signatures
    my $cnt;
    for $var (@_) {
        $cnt += $var;
    }

    return $cnt;
}

$left = 1;
$right = 3;
printf("left is:%d\n",$left);
$sum = sumlr($left,$right);
printf("sum(1,3) is:%d\n",$sum);
printf("sum(1,3) is:%d\n",sumlr($left,$right));
printf("sum(1,3) is:%d\n",sumlr2($left,$right));

$line = "abc\n";
$line_ref = \$line;                             #得到$line的ref https://perldoc.perl.org/perlreftut

{ package mypk;
    sub test {
        my ($class, $line) = @_;
        printf("\$line:%s\n",$line);
        printf("\$\$line:%s\n",$$line);        #
    }
}

#test(\$line);
mypk->test($line);
mypk->test(\$line);
