#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/TaskGridCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/TaskGridCmdTest
mkdir -p $OUTPUT_DIR 

echo "node density"
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density.geojson --maxNodesPerCell 1000 --pixelSize 0.001 --node-density
diff $GOLD_FILES_DIR/output-node-density.geojson $OUTPUT_DIR/output-node-density.geojson
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density.osm --maxNodesPerCell 1000 --pixelSize 0.001 --node-density
diff $GOLD_FILES_DIR/output-node-density.osm $OUTPUT_DIR/output-node-density.osm

echo "node density random"
hoot task-grid --error -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-random.geojson --maxNodesPerCell 1000 --random --randomSeed 1 --node-density
diff $GOLD_FILES_DIR/output-node-density-random.geojson $OUTPUT_DIR/output-node-density-random.geojson
hoot task-grid --error -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-random.osm --maxNodesPerCell 1000 --random --randomSeed 1 --node-density
diff $GOLD_FILES_DIR/output-node-density-random.osm $OUTPUT_DIR/output-node-density-random.osm

# small map test - total nodes = ~7k
echo "node density small"
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-small.geojson --maxNodesPerCell 10000 --node-density
diff $GOLD_FILES_DIR/output-node-density-small.geojson $OUTPUT_DIR/output-node-density-small.geojson
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-small.osm --maxNodesPerCell 10000 --node-density
diff $GOLD_FILES_DIR/output-node-density-small.osm $OUTPUT_DIR/output-node-density-small.osm

# auto retry test - takes four attempts
echo "node density auto retry"
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-retry.geojson --maxNodesPerCell 500 --pixelSize 0.001 --maxAttempts 4 --pixelSizeReductionFactor 50 --node-density
diff $GOLD_FILES_DIR/output-node-density-retry.geojson $OUTPUT_DIR/output-node-density-retry.geojson
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-retry.osm --maxNodesPerCell 500 --pixelSize 0.001 --maxAttempts 4 --pixelSizeReductionFactor 50 --node-density
diff $GOLD_FILES_DIR/output-node-density-retry.osm $OUTPUT_DIR/output-node-density-retry.osm

echo "node density bounds"
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-bounds.geojson --maxNodesPerCell 1000 --pixelSize 0.001 --bounds "-77.0403,38.8914,-77.0333,38.8959" --node-density
diff $GOLD_FILES_DIR/output-node-density-bounds.geojson $OUTPUT_DIR/output-node-density-bounds.geojson
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-node-density-bounds.osm --maxNodesPerCell 1000 --pixelSize 0.001 --bounds "-77.0403,38.8914,-77.0333,38.8959" --node-density
diff $GOLD_FILES_DIR/output-node-density-bounds.osm $OUTPUT_DIR/output-node-density-bounds.osm

echo "uniform"
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-uniform.geojson 4 --uniform
diff $GOLD_FILES_DIR/output-uniform.geojson $OUTPUT_DIR/output-uniform.geojson
hoot task-grid --warn -C Testing.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output-uniform.osm 4 --uniform
diff $GOLD_FILES_DIR/output-uniform.osm $OUTPUT_DIR/output-uniform.osm

echo "uniform bounds"
hoot task-grid --warn -C Testing.conf "-77.0403,38.8914,-77.0333,38.8959" $OUTPUT_DIR/output-uniform-bounds.geojson 2 --uniform
diff $GOLD_FILES_DIR/output-uniform-bounds.geojson $OUTPUT_DIR/output-uniform-bounds.geojson
hoot task-grid --warn -C Testing.conf "-77.0403,38.8914,-77.0333,38.8959" $OUTPUT_DIR/output-uniform-bounds.osm 2 --uniform
diff $GOLD_FILES_DIR/output-uniform-bounds.osm $OUTPUT_DIR/output-uniform-bounds.osm
