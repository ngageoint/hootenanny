#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/StatCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/visitors/SumNumericTagsVisitorTest.osm
INPUT_FILE_2=test-files/ToyBuildingsTestA.osm
RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Calculating the numerical average of all values for a custom tag..."
echo ""
hoot stat $LOG_LEVEL $CONFIG -D tags.visitor.keys="test1" $INPUT_FILE_1 AverageNumericTagsVisitor

echo ""
echo "Calculating the minimum number of nodes per way..."
echo ""
hoot stat $LOG_LEVEL $CONFIG $INPUT_FILE_2 NodesPerWayVisitor min

echo ""
echo "Calculating the maximum number of nodes per way..."
echo ""
hoot stat $LOG_LEVEL $CONFIG $INPUT_FILE_2 NodesPerWayVisitor max

echo ""
echo "Calculating the average number of nodes per way..."
echo ""
hoot stat $LOG_LEVEL $CONFIG $INPUT_FILE_2 NodesPerWayVisitor average

echo ""
echo "Calculating the total number of nodes per way against multiple files..."
echo ""
hoot stat $LOG_LEVEL $CONFIG "$INPUT_FILE_1;$INPUT_FILE_2" NodesPerWayVisitor total

echo ""
echo "Calculating the total number of nodes per way recursively from a directory filtered to read osm inputs only..."
echo ""
hoot stat $LOG_LEVEL $CONFIG $RECURSIVE_INPUT NodesPerWayVisitor total --recursive "*.osm"
