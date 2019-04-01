#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/CleanCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/CleanCmdTest/cleaned.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/CleanCmdTest/cleaned.osm

# Run the command.
hoot clean -C Testing.conf $inputfile $outputfile

# Checking output
hoot diff $comparefile $outputfile || diff $comparefile $outputfile
