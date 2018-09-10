#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/StatCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/visitors/SumNumericTagsVisitorTest.osm

# calculate the numerical average of all values for a custom tag
hoot stat -D tags.visitor.keys="test1" $INPUT_FILE_1 hoot::AverageNumericTagsVisitor
