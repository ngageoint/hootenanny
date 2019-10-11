#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/NodeDensityTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/NodeDensityTilesCmdTest
mkdir -p $OUTPUT_DIR 

hoot node-density-tiles --warn "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.geojson 1000 0.001
diff $GOLD_FILES_DIR/output.geojson $OUTPUT_DIR/output.geojson

hoot node-density-tiles --error "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output-random.geojson 1000 0.001 1 --random
diff $GOLD_FILES_DIR/output-random.geojson $OUTPUT_DIR/output-random.geojson

