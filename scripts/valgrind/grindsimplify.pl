#!/usr/bin/perl
# This file is part of Hootenanny.
#
# Hootenanny is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# --------------------------------------------------------------------
#
# The following copyright notices are generated automatically. If you
# have a new notice to add, please use the format:
# " * @copyright Copyright ..."
# This will properly maintain the copyright information. Maxar
# copyrights will be updated automatically.
#
# @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)

use warnings;
use strict;

my $output = "";
my $in_error = 0;
my $error = "";
my $good_error = 0;
my $error_id = "";
my %errors_removed;
my $in_stack = 0;
my $stack = "";
my $first_stack_done = 0;
my $in_counts = 0;
my $in_frame = 0;
my $good_frame = 0;
my $has_obj = 0;
my $has_fn = 0;
my $has_dir = 0;
my $has_file = 0;
my $has_line = 0;
my $in_pair = 0;
my $pair = "";
my $good_pair = 0;

my @lines;
my $line;

my $FILEHANDLE;

if(@ARGV > 1) {
  print STDERR ("Usage: grindsimplify.pl <XML filename>");
  exit 1;
}

unless (open $FILEHANDLE, "<:encoding(utf8)", $ARGV[0]) {
  print STDERR "Could not open file '$ARGV[0]': $!\n";
  return undef;
}
chomp(@lines = <$FILEHANDLE>);
unless (close $FILEHANDLE) {
  print STDERR "Error while closing file '$ARGV[0]': $!\n";
  return undef;
}

foreach $line (@lines) {
  if ($line =~ /.+/) {
    if ($line =~ /<error>/) {
      $in_error = 1;
      $good_error = 0;
      $first_stack_done = 0;
      $error = "$line\n";
    }
    elsif ($in_error == 0) {
      if ($line =~ /<errorcounts>/) {
        $in_counts = 1;
        $output .= "$line\n";
      }
      elsif ($line =~ /<\/errorcounts>/) {
        $in_counts = 0;
        $output .= "$line\n";
      }
      elsif ($in_counts == 1) {
        # Only output pairs that don't exist in the %errors_removed hash
        if ($line =~ /<pair>/) {
          $in_pair = 1;
          $pair = "$line\n";
          $good_pair = 0;
        }
        elsif ($line =~ /<\/pair>/) {
          $in_pair = 0;
          $pair .= "$line\n";
          # Output the good pair
          if ($good_pair == 1) {
            $output .= $pair;
          }
        }
        elsif ($in_pair == 1) {
          $pair .= "$line\n";
          if ($line =~ /<unique>(.+)<\/unique>/) {
            # Check if the unique ID isn't found, that means is wasn't removed
            if (!exists($errors_removed{$1})) {
              $good_pair = 1;
            }
          }
        }
      }
      else {
        $output .= "$line\n";
      }
    }
    else {
      $error .= "$line\n";
      if ($line =~ /<\/error>/) {
        $in_error = 0;
        # Good errors get written out to the output, while bad errors are removed from here and the error counts later
        if ($good_error == 1) {
          $output .= $error;
        }
        else {
          $errors_removed{$error_id} = 1;
        }
      }
      elsif ($line =~ /<unique>(.*)<\/unique>/) {
        $error_id = $1;
      }
      elsif ($line =~ /<stack>/) {
        $in_stack = 1;
        $good_frame = 0;
      }
      elsif ($line =~ /<\/stack>/) {
        $in_stack = 0;
        $first_stack_done = 1;
        # If the previous frame was good, the error is good
        if ($good_frame == 1) {
          $good_error = 1;
        }
      }
      # Only search the first stack and not anything after the <auxwhay> tag
      elsif ($in_stack == 1 && $first_stack_done == 0) {
        if ($line =~ /<frame>/) {
          $in_frame = 1;
          $has_obj = $has_fn = $has_dir = $has_file = $has_line = 0;
          # If the previous frame was good, the error is good
          if ($good_frame == 1) {
            $good_error = 1;
          }
          $good_frame = 0;
        }
        elsif ($line =~ /<\/frame>/) {
          $in_frame = 0;
          # Good frames have obj, fn, dir, file, or line information, not just ip only
          if ($has_obj == 1 || $has_fn == 1 || $has_dir == 1 || $has_file == 1 || $has_line == 1) {
            $good_frame = 1;
          }
        }
        elsif ($in_frame == 1) {
          # One of these 5 non-empty XML tags makes the frame a good frame
          if ($line =~ /<obj>.+<\/obj>/) {
            $has_obj = 1;
          }
          elsif ($line =~ /<fn>.+<\/fn>/) {
            $has_fn = 1;
          }
          elsif ($line =~ /<dir>.+<\/dir>/) {
            $has_dir = 1;
          }
          elsif ($line =~ /<file>.+<\/file>/) {
            $has_file = 1;
          }
          elsif ($line =~ /<line>.+<\/line>/) {
            $has_line = 1;
          }
        }
      }
    }
  }
}

unless (open $FILEHANDLE, ">:encoding(utf8)", $ARGV[0]) {
  print STDERR "Could not re-open file '$ARGV[0]': $!\n";
  return undef;
}
print $FILEHANDLE $output;
unless (close $FILEHANDLE) {
  print STDERR "Error while closing file '$ARGV[0]': $!\n";
  return undef;
}

exit 0;
