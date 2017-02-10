#!/bin/bash
set -e

# Test reading from files and producing stats.

INPUT1=test-files/conflate/unified/AllDataTypesA.osm
INPUT2=test-files/conflate/unified/AllDataTypesB.osm
OUTPUT1=test-output/StatsAllDataTypesA.txt
OUTPUT2=test-output/StatsAllDataTypes.json

rm -f $OUTPUT1 $OUTPUT2

# Test the stats
echo "****Quick Stats****"
hoot stats --brief --output=$OUTPUT1 $INPUT1
cat $OUTPUT1

echo "****JSON Stats****"
hoot stats --output=$OUTPUT2 $INPUT1 $INPUT2
cat $OUTPUT2

