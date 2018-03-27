#!/bin/bash
set -e

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep multiary-conflate`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "multiary-conflate command not present.  Exiting test without running it."
 exit 0
fi

mkdir -p $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest

export TESTDIR=$HOOT_HOME/test-files/cmd/slow/MultiaryConflateCmdTest

# Conflate 3 independant files (no merging) and verify that the status values are correct.
hoot multiary-conflate  -D writer.include.debug.tags=true $TESTDIR/Input1.osm $TESTDIR/Input2.osm $TESTDIR/Input3.osm $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm

# Verify we get one of each status type out.
grep 'k="hoot:status" v="Input003"' $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm
grep 'k="hoot:status" v="1"' $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm
grep 'k="hoot:status" v="2"' $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm
