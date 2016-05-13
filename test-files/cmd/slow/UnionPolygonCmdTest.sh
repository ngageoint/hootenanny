#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/UnionPolygonCmdTest

inputfile1=$HOOT_HOME/test-files/square.osm
inputfile2=$HOOT_HOME/test-files/diamond.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/UnionPolygonCmdTest/union.osm
comparefile=$HOOT_HOME/test-files/cmd/slow/UnionPolygonCmdTest/union.osm

# Run the command.
hoot union-polygons $outputfile $inputfile1 $inputfile2

# Checking output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile
