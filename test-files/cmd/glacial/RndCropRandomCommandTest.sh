#!/bin/bash
set -e

INPUT_DIR=$HOOT_HOME/test-files/cmd/glacial/CropRandomCmdTest
OUTPUT_DIR=$HOOT_HOME/test-output/cmd/glacial/CropRandomCmdTest
mkdir -p $OUTPUT_DIR

hoot crop-random --warn -D crop.keep.only.features.inside.bounds=true "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/out.osm 1000 0.001 1 --write-tiles
hoot diff --warn $OUTPUT_DIR/out.osm $INPUT_DIR/out.osm || diff $OUTPUT_DIR/out.osm $INPUT_DIR/out.osm
hoot diff --warn $OUTPUT_DIR/out-tiles.osm $INPUT_DIR/out-tiles.osm || diff $OUTPUT_DIR/out-tiles.osm $INPUT_DIR/out-tiles.osm
