#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/CleanCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/CleanCmdTest/cleaned.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/CleanCmdTest/cleaned.osm

CONFIG="-C Testing.conf"

# Run the command.
hoot clean $CONFIG $inputfile $outputfile

# Checking output
hoot diff $CONFIG $comparefile $outputfile || diff $comparefile $outputfile
