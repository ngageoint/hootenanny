#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/CleanupCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
comparefile=$HOOT_HOME/hoot-core-test/src/test/resources/cmd/slow/CleanupCmdTest/cleaned.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/CleanupCmdTest/cleaned.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot cleanup $inputfile $outputfile | \
             sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g"

# Checking output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile
