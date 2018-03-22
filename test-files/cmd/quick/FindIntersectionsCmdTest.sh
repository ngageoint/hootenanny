#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/FindIntersectionsCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/FindIntersectionsCmdTest/out.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/FindIntersectionsCmdTest/out.osm

# Run the command.
hoot find-intersections $inputfile $outputfile

# Checking output
hoot map-diff $comparefile $outputfile || diff $comparefile $outputfile
