#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/RndMultiaryScorePoiMatchesCmdTest

export TESTDIR=$HOOT_HOME/test-files/cmd/glacial/RndMultiaryScorePoiMatchesCmdTest

# Conflate 3 independant files (no merging) and verify that the status values are correct.
hoot multiary-score-poi-matches -C Testing.conf --confusion $TESTDIR/Input1.osm $TESTDIR/Input2.osm $TESTDIR/Input3.osm $HOOT_HOME/test-output/cmd/glacial/RndMultiaryScorePoiMatchesCmdTest/Output.osm

# Verify we get some wrong tags in the output
grep 'k="hoot:wrong" v="missMatch"' $HOOT_HOME/test-output/cmd/glacial/RndMultiaryScorePoiMatchesCmdTest/Output.osm
