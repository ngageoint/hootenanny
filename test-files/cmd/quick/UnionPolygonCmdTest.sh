#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/UnionPolygonCmdTest

inputfile1=$HOOT_HOME/test-files/square.osm
inputfile2=$HOOT_HOME/test-files/diamond.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/UnionPolygonCmdTest/union.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/UnionPolygonCmdTest/union.osm

# Run the command.
hoot union-polygons $outputfile $inputfile1 $inputfile2

# Checking output
hoot map-diff $comparefile $outputfile || diff $comparefile $outputfile
