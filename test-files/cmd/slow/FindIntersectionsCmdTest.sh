#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/FindIntersectionsCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/slow/FindIntersectionsCmdTest/out.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/FindIntersectionsCmdTest/out.osm

# Run the command.
hoot find-intersections $inputfile $outputfile

# Checking output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile
