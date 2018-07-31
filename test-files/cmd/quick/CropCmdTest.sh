#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/CropCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/CropCmdTest/cropped.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/CropCmdTest/cropped.osm

# Run the command.
hoot crop $inputfile $outputfile -77.05,38.888,-77.03,38.90 

# Checking output
hoot diff $comparefile $outputfile || diff $comparefile $outputfile
