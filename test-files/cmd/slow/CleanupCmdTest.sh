#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/CleanupCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/test-files/cmd/slow/CleanupCmdTest/cleaned.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/CleanupCmdTest/cleaned.osm

# Run the command.
hoot cleanup $inputfile $outputfile

# Checking output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile
