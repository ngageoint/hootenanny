#!/bin/bash
set -e

TEST_PATH="cmd/slow/AlignApplyCmdTest"
INPUT_PATH="$HOOT_HOME/test-files/$TEST_PATH"
OUTPUT_PATH="$HOOT_HOME/test-output/$TEST_PATH"

mkdir -p $HOOT_HOME/test-output/cmd/slow/AlignApplyCmdTest

CONFIG="-C Testing.conf"

# First we derive a rubber sheet
hoot align --error $CONFIG --derive --ref \
  $HOOT_HOME/test-files/DcGisRoads.osm \
  $HOOT_HOME/test-files/DcTigerRoads.osm \
  $OUTPUT_PATH/DcTigerToDcGis.rs

# Compare to known-good
goodfile=$INPUT_PATH/DcTigerToDcGis.rs
testfile=$OUTPUT_PATH/DcTigerToDcGis.rs
cmp $goodfile $testfile
if [ "$?" = "1" ]; then
  echo "Rubber Sheet files are not equal! Try cmp -l $goodfile $testfile"
  exit 1
fi

# Now we apply the rubber sheet
hoot align --error $CONFIG --apply \
  $OUTPUT_PATH/DcTigerToDcGis.rs \
  $HOOT_HOME/test-files/DcTigerRoads.osm \
  $OUTPUT_PATH/DcTigerToDcGis.osm

goodfile=$INPUT_PATH/DcTigerToDcGis.osm
testfile=$OUTPUT_PATH/DcTigerToDcGis.osm
hoot diff $CONFIG $goodfile $testfile || diff $goodfile $testfile
