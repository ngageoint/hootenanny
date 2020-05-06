#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/NodeDensityTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/NodeDensityTilesCmdTest
mkdir -p $OUTPUT_DIR 

hoot node-density-tiles --warn -C Testing.conf "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.geojson 1000 --pixel-size 0.001
diff $GOLD_FILES_DIR/output.geojson $OUTPUT_DIR/output.geojson

# random test
hoot node-density-tiles --error -C Testing.conf "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output-random.geojson 1000 --random --random-seed 1
diff $GOLD_FILES_DIR/output-random.geojson $OUTPUT_DIR/output-random.geojson

# small map test
#hoot node-density-tiles --warn -C Testing.conf "test-files/DcGisRoads.osm" $OUTPUT_DIR/output-small.geojson 1 --pixel-size 0.001
#diff $GOLD_FILES_DIR/output-small.geojson $OUTPUT_DIR/output-small.geojson

# auto retry test
#hoot node-density-tiles --warn -C Testing.conf "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.geojson 100 --pixel-size 0.01 --max-attempts 2 --maxNodeCountPerTileAutoIncreaseFactor 3 --pixelSizeAutoReductionFactor 2.5
#diff $GOLD_FILES_DIR/output-retry.geojson $OUTPUT_DIR/output-retry.geojson
