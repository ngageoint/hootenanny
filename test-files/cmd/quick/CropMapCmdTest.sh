#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/CropMapCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/CropMapCmdTest/cropped.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/CropMapCmdTest/cropped.osm

# Run the command.
hoot crop $inputfile $outputfile -77.05,38.888,-77.03,38.90 

# Checking output
hoot map-diff $comparefile $outputfile || diff $comparefile $outputfile
