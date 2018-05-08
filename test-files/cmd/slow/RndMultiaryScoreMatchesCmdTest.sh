#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/RndMultiaryScoreMatchesCmdTest

export TESTDIR=$HOOT_HOME/test-files/cmd/slow/RndMultiaryScoreMatchesCmdTest

# Conflate 3 independant files (no merging) and verify that the status values are correct.
hoot multiary-score-matches --confusion $TESTDIR/Input1.osm $TESTDIR/Input2.osm $TESTDIR/Input3.osm $HOOT_HOME/test-output/cmd/slow/RndMultiaryScoreMatchesCmdTest/Output.osm

# Verify we get some wrong tags in the output
grep 'k="hoot:wrong" v="missMatch"' $HOOT_HOME/test-output/cmd/slow/RndMultiaryScoreMatchesCmdTest/Output.osm
