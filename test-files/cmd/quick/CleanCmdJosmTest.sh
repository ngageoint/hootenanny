#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/CleanCmdJosmTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/CleanCmdJosmTest/cleaned.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/CleanCmdJosmTest/cleaned.osm

# Run the command.
hoot clean -C Testing.conf -D map.cleaner.transforms="hoot::JosmMapCleaner" $inputfile $outputfile

# Checking output
hoot diff $comparefile $outputfile || diff $comparefile $outputfile
