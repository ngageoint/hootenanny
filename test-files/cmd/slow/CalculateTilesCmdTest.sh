#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/CalculateTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/CalculateTilesCmdTest
mkdir -p $OUTPUT_DIR
echo $OUTPUT_DIR 

hoot calculate-tiles --warn "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" "$OUTPUT_DIR/output.geojson" 1000 0.001
diff "$GOLD_FILES_DIR/output.geojson" "$OUTPUT_DIR/output.geojson"

