#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/MultiaryPoiConflateCmdTest

export TESTDIR=$HOOT_HOME/test-files/cmd/slow/MultiaryPoiConflateCmdTest

# Conflate 3 independant files (no merging) and verify that the status values are correct.
hoot multiary-poi-conflate -C Testing.conf -D writer.include.debug.tags=true $TESTDIR/Input1.osm $TESTDIR/Input2.osm $TESTDIR/Input3.osm $HOOT_HOME/test-output/cmd/slow/MultiaryPoiConflateCmdTest/Output.osm

# Verify we get one of each status type out.
grep 'k="hoot:status" v="Input003"' $HOOT_HOME/test-output/cmd/slow/MultiaryPoiConflateCmdTest/Output.osm
grep 'k="hoot:status" v="Input1"' $HOOT_HOME/test-output/cmd/slow/MultiaryPoiConflateCmdTest/Output.osm
grep 'k="hoot:status" v="Input2"' $HOOT_HOME/test-output/cmd/slow/MultiaryPoiConflateCmdTest/Output.osm
