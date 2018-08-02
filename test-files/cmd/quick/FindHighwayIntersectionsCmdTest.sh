#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/FindHighwayIntersectionsCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/FindHighwayIntersectionsCmdTest/out.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/FindHighwayIntersectionsCmdTest/out.osm

# Run the command.
hoot find-intersections $inputfile $outputfile

# Checking output
hoot diff $comparefile $outputfile || diff $comparefile $outputfile
