#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/CropMapCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/hoot-core-test/src/test/resources/cmd/slow/CropMapCmdTest/cropped.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/CropMapCmdTest/cropped.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot crop-map $inputfile $outputfile -77.05,38.888,-77.03,38.90 | \
              sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g"

# Checking output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile
