#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/NodeDensityTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/NodeDensityTilesCmdTest
mkdir -p $OUTPUT_DIR 

hoot node-density-tiles --warn -C Testing.conf "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.geojson --maxNodesPerTile 1000 --pixelSize 0.001
diff $GOLD_FILES_DIR/output.geojson $OUTPUT_DIR/output.geojson

# random test
hoot node-density-tiles --error -C Testing.conf "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output-random.geojson --maxNodesPerTile 1000 --random --randomSeed 1
diff $GOLD_FILES_DIR/output-random.geojson $OUTPUT_DIR/output-random.geojson

# small map test - total nodes = ~7k
hoot node-density-tiles --warn -C Testing.conf "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output-small.geojson --maxNodesPerTile 10000
diff $GOLD_FILES_DIR/output-small.geojson $OUTPUT_DIR/output-small.geojson

# auto retry test - takes four attempts
hoot node-density-tiles --warn -C Testing.conf "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output-retry.geojson --maxNodesPerTile 500 --pixelSize 0.001 --maxAttempts 4 --pixelSizeReductionFactor 50
diff $GOLD_FILES_DIR/output-retry.geojson $OUTPUT_DIR/output-retry.geojson

