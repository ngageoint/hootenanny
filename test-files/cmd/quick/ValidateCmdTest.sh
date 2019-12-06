#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/ValidateCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/ValidateCmdTest/validated.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/ValidateCmdTest/validated.osm

# Run the command.
hoot validate -C Testing.conf $inputfile $outputfile

# Checking output
hoot diff $comparefile $outputfile || diff $comparefile $outputfile
