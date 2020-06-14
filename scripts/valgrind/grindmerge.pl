#!/usr/bin/perl

# Copyright 2016 Timothe Litt litt at acm _ddot_ org
#
# May be freely used and copied providing this notice is retained
# No warranty, use at your own risk
#

# valgrind foo 3>&1 1>&2 2>&3 | grindmerge -f input >newrules

use warnings;
use strict;

use Digest::MD5 qw/md5_hex/;

my %known;
my $dups = 0;

sub parse {
    my $in = shift;

    while( <$in> ) {
        next unless( /^\{/ );

        my $block = $_;
        while( <$in> ) {
            if( /^\}/ ) {
                $block .= "}\n";
                last;
            }
            $block .= $_;
        }
        last unless( defined $block );
        if( $block !~ /\}\n/ ) {
            print STDERR ("Unterminated suppression at line $.\n" );
            last;
        }
        my $key = $block;
        $key =~ s/(\A{[^\n]*\n)\s*[^\n]*\n/$1/;
        my $sum = md5_hex( $key );
        $dups++ if( exists $known{$sum} );
        $known{$sum} = $block;
    }
}

if( @ARGV >= 2 && $ARGV[0] eq '-f' ) {
    if( open( my $db, '<', $ARGV[1] ) ) {
        parse( $db );
        close( $db );
    } else {
        print STDERR ("Open failed for $ARGV[1]: $!\n");
        exit 1;
    }
    print STDERR ("Read " . keys( %known ) . " suppressions from $ARGV[1]\n" );
}

parse( \*STDIN );

print $known{$_} foreach ( sort keys %known );

print STDERR ("Squashed $dups duplicate suppressions\n")
