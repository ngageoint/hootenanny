#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/StatCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/visitors/SumNumericTagsVisitorTest.osm
INPUT_FILE_2=test-files/ToyBuildingsTestA.osm

CONFIG="-C Testing.conf"

# calculate the numerical average of all values for a custom tag
hoot stat $CONFIG -D tags.visitor.keys="test1" $INPUT_FILE_1 hoot::AverageNumericTagsVisitor

# calculate the averge number of nodes per way
hoot stat $CONFIG $INPUT_FILE_2 hoot::NodesPerWayVisitor average

# This results in an error since AddressCountVisitor doesn't support NumericStatistic.
hoot stat $CONFIG $INPUT_FILE_2 hoot::AddressCountVisitor average
