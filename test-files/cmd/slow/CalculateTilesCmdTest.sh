#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/CalculateTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/CalculateTilesCmdTest
mkdir -p $OUTPUT_DIR 

hoot calculate-tiles "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output 1000 0.001
diff $GOLD_FILES_DIR/output test-output/cmd/slow/CalculateTilesCmdTest/output

hoot calculate-tiles "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.osm 1000 0.001
diff $GOLD_FILES_DIR/output.osm $OUTPUT_DIR/output.osm

