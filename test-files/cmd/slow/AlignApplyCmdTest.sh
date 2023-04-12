#!/bin/bash
set -e

TEST_PATH="cmd/slow/AlignApplyCmdTest"
INPUT_PATH="$HOOT_HOME/test-files/$TEST_PATH"
OUTPUT_PATH="$HOOT_HOME/test-output/$TEST_PATH"

mkdir -p $OUTPUT_PATH

CONFIG="-C Testing.conf"

derive_and_apply() {
  INPUT1=$1
  INPUT2=$2
  RUBBERSHEET=$3
  GOLD_RUBBERSHEET=$4
  OUTPUT_FILE=$5
  GOLD_OUTPUT=$6
  BUILDING=$7

  # First we derive a rubber sheet
  hoot align --error $CONFIG -D rubber.sheet.use.buildings=$BUILDING --derive \
    $INPUT1 \
    $INPUT2 \
    $RUBBERSHEET --ref

  # Compare to known-good
  cmp $GOLD_RUBBERSHEET $RUBBERSHEET
  if [ "$?" = "1" ]; then
    echo "Rubber Sheet files are not equal! Try cmp -l $GOLD_RUBBERSHEET $RUBBERSHEET"
    exit 1
  fi

  # Now we apply the rubber sheet
  hoot align --error $CONFIG --apply \
    $RUBBERSHEET \
    $INPUT2 \
    $OUTPUT_FILE

  hoot diff $CONFIG $GOLD_OUTPUT $OUTPUT_FILE || diff $GOLD_OUTPUT $OUTPUT_FILE
}

# First derive and apply a road network
derive_and_apply $HOOT_HOME/test-files/DcGisRoads.osm \
                 $HOOT_HOME/test-files/DcTigerRoads.osm \
                 $OUTPUT_PATH/DcTigerToDcGis.rs \
                 $INPUT_PATH/DcTigerToDcGis.rs \
                 $OUTPUT_PATH/DcTigerToDcGis.osm \
                 $INPUT_PATH/DcTigerToDcGis.osm \
                 false

# Then derive and apply a building file
derive_and_apply $INPUT_PATH/ToyBuildingsTest1.osm \
                 $INPUT_PATH/ToyBuildingsTest2.osm \
                 $OUTPUT_PATH/ToyBuildingsTest.rs \
                 $INPUT_PATH/ToyBuildingsTest.rs \
                 $OUTPUT_PATH/ToyBuildingsTest.osm \
                 $INPUT_PATH/ToyBuildingsTest.osm \
                 true
