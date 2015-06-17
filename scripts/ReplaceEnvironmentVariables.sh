#!/bin/bash
set -e

#
# Source $HOOT_HOME/conf/DatabaseConfig.sh to get the current DB settings as env. variables.
# Given an input file, $1, replace any instance of ${X} with the corresponding environment variable
# and write the result to $2.
#

if [ ! -f $HOOT_HOME/conf/DatabaseConfig.sh ]; then
    echo "Using default database configuration. You can safely customize: $HOOT_HOME/conf/DatabaseConfig.sh"
    cp $HOOT_HOME/conf/DatabaseConfig.sh.orig $HOOT_HOME/conf/DatabaseConfig.sh
fi
source $HOOT_HOME/conf/DatabaseConfig.sh

perl -p -e 's/\$\{([^}]+)\}/defined $ENV{$1} ? $ENV{$1} : $&/eg; s/\$\{}//eg' $1 > $2

