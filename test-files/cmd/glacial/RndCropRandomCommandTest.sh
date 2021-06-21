#!/bin/bash
set -e

INPUT_DIR=$HOOT_HOME/test-files/cmd/glacial/RndCropRandomCmdTest
OUTPUT_DIR=$HOOT_HOME/test-output/cmd/glacial/RndCropRandomCmdTest
mkdir -p $OUTPUT_DIR

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

hoot crop-random $LOG_LEVEL $CONFIG -D crop.keep.only.features.inside.bounds=true test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/out.osm 1000 0.001 --randomSeed 1 --write-tiles
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/out.osm $INPUT_DIR/out.osm || diff $OUTPUT_DIR/out.osm $INPUT_DIR/out.osm
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/out-tiles.osm $INPUT_DIR/out-tiles.osm || diff $OUTPUT_DIR/out-tiles.osm $INPUT_DIR/out-tiles.osm
