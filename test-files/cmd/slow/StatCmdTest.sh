#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/StatCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/visitors/SumNumericTagsVisitorTest.osm
INPUT_FILE_2=test-files/ToyBuildingsTestA.osm

CONFIG="-C Testing.conf"

echo ""
echo "Calculating the numerical average of all values for a custom tag..."
echo ""
hoot stat $CONFIG -D tags.visitor.keys="test1" $INPUT_FILE_1 hoot::AverageNumericTagsVisitor

echo ""
echo "Calculating the minimum number of nodes per way..."
echo ""
hoot stat $CONFIG $INPUT_FILE_2 NodesPerWayVisitor min

echo ""
echo "Calculating the maximum number of nodes per way..."
echo ""
hoot stat $CONFIG $INPUT_FILE_2 NodesPerWayVisitor max

echo ""
echo "Calculating the average number of nodes per way..."
echo ""
hoot stat $CONFIG $INPUT_FILE_2 NodesPerWayVisitor average

echo ""
echo "Calculating the total number of nodes per way against multiple files..."
echo ""
hoot stat $CONFIG "$INPUT_FILE_1;$INPUT_FILE_2" NodesPerWayVisitor total
