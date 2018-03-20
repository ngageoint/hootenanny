#!/bin/bash
set -e

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep multiary-score-matches`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "multiary-score-matches command not present.  Exiting test without running it."
 exit 0
fi

mkdir -p $HOOT_HOME/test-output/cmd/slow/MultiaryScoreMatchesCmdTest

export TESTDIR=$HOOT_HOME/test-files/cmd/slow/MultiaryScoreMatchesCmdTest

# Conflate 3 independant files (no merging) and verify that the status values are correct.
hoot multiary-score-matches --confusion $TESTDIR/Input1.osm $TESTDIR/Input2.osm $TESTDIR/Input3.osm $HOOT_HOME/test-output/cmd/slow/MultiaryScoreMatchesCmdTest/Output.osm

# Verify we get some wrong tags in the output
grep 'k="hoot:wrong" v="missMatch"' $HOOT_HOME/test-output/cmd/slow/MultiaryScoreMatchesCmdTest/Output.osm
