#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/RndCalculateTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/RndCalculateTilesCmdTest
mkdir -p $OUTPUT_DIR 

hoot calculate-tiles --warn "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.geojson 1000 0.001
diff $GOLD_FILES_DIR/output.geojson $OUTPUT_DIR/output.geojson

hoot calculate-tiles --error "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output-random.geojson 1000 0.001 1 --random
diff $GOLD_FILES_DIR/output-random.geojson $OUTPUT_DIR/output-random.geojson

