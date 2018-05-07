#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest

export TESTDIR=$HOOT_HOME/test-files/cmd/slow/MultiaryConflateCmdTest

# Conflate 3 independant files (no merging) and verify that the status values are correct.
hoot multiary-conflate  -D writer.include.debug.tags=true $TESTDIR/Input1.osm $TESTDIR/Input2.osm $TESTDIR/Input3.osm $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm

# Verify we get one of each status type out.
grep 'k="hoot:status" v="Input003"' $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm
grep 'k="hoot:status" v="1"' $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm
grep 'k="hoot:status" v="2"' $HOOT_HOME/test-output/cmd/slow/MultiaryConflateCmdTest/Output.osm
