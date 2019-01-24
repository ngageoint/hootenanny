#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/RndMultiaryPoiConflateCmdTest

export TESTDIR=$HOOT_HOME/test-files/cmd/glacial/RndMultiaryPoiConflateCmdTest

# Conflate 3 independant files (no merging) and verify that the status values are correct.
hoot multiary-poi-conflate  -D writer.include.debug.tags=true $TESTDIR/Input1.osm $TESTDIR/Input2.osm $TESTDIR/Input3.osm $HOOT_HOME/test-output/cmd/glacial/RndMultiaryPoiConflateCmdTest/Output.osm

# Verify we get one of each status type out.
grep 'k="hoot:status" v="Input003"' $HOOT_HOME/test-output/cmd/glacial/RndMultiaryPoiConflateCmdTest/Output.osm
grep 'k="hoot:status" v="1"' $HOOT_HOME/test-output/cmd/glacial/RndMultiaryPoiConflateCmdTest/Output.osm
grep 'k="hoot:status" v="2"' $HOOT_HOME/test-output/cmd/glacial/RndMultiaryPoiConflateCmdTest/Output.osm
