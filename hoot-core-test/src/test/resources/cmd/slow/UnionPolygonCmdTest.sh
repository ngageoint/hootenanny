#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/UnionPolygonCmdTest

inputfile1=$HOOT_HOME/test-files/square.osm
inputfile2=$HOOT_HOME/test-files/diamond.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/UnionPolygonCmdTest/union.osm
comparefile=$HOOT_HOME/hoot-core-test/src/test/resources/cmd/slow/UnionPolygonCmdTest/union.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot union-polygons $outputfile $inputfile1 $inputfile2  | \
              sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g"

# Checking output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile
