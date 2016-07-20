#!/bin/bash

# By doing the clean db in hoot-services we don't need to run configure.
#cd $HOOT_HOME/hoot-services/
#make clean-db &> /dev/null

# Clean sometimes refuses to delete these directories. Odd.
cd $HOOT_HOME

rm -rf docs/node_modules hoot-core/tmp/ hoot-core-test/tmp tgs/tmp

git clean -d -f -f -x || echo "It is ok if this fails, it sometimes mysteriously doesn't clean"
