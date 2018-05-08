#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/RndCalculateRandomTileCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/RndCalculateRandomTileCmdTest
mkdir -p $OUTPUT_DIR

hoot tiles-calculate-random --error "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.geojson 1 1000 0.001
diff $GOLD_FILES_DIR/output.geojson $OUTPUT_DIR/output.geojson

