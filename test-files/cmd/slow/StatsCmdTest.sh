#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/StatsFileTest

# Test reading from files and producing stats.

INPUT1=test-files/conflate/unified/AllDataTypesA.osm
INPUT2=test-files/conflate/unified/AllDataTypesB.osm
OUTPUT1=$OUTPUT_DIR/StatsAllDataTypesA.txt
OUTPUT2=$OUTPUT_DIR/StatsAllDataTypes.json
RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

echo ""
echo "Quick Stats from a single file to the display..."
echo ""
hoot stats $LOG_LEVEL $CONFIG $INPUT1 --brief

echo ""
echo "Quick Stats from a single file to txt..."
echo ""
hoot stats $LOG_LEVEL $CONFIG $INPUT1 $OUTPUT1 --brief
cat $OUTPUT1

echo ""
echo "Full stats from multiple files to json..."
echo ""
hoot stats $LOG_LEVEL $CONFIG "$INPUT1;$INPUT2" $OUTPUT2 
cat $OUTPUT2

echo ""
echo "Quick Stats recursively from a directory to the display..."
echo ""
hoot stats $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --brief --recursive "*"

echo ""
echo "Quick Stats recursively from a directory filtered to read json inputs only to the display..."
echo ""
hoot stats $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --brief --recursive "*.json"

