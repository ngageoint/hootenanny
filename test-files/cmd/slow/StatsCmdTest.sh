#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/StatsFileTest

# Test reading from files and producing stats.

INPUT1=test-files/conflate/unified/AllDataTypesA.osm
INPUT2=test-files/conflate/unified/AllDataTypesB.osm
OUTPUT1=$OUTPUT_DIR/StatsAllDataTypesA.txt
OUTPUT2=$OUTPUT_DIR/StatsAllDataTypes.json

CONFIG="-C Testing.conf"

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

# Test the stats
echo "****Quick Stats****"
hoot stats $CONFIG $INPUT1 $OUTPUT1 --brief
cat $OUTPUT1

echo "****JSON Stats****"
hoot stats $CONFIG "$INPUT1;$INPUT2" $OUTPUT2 
cat $OUTPUT2

