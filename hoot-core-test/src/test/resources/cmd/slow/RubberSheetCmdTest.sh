#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/RubberSheetCmdTest

input1=$HOOT_HOME/test-files/DcGisRoads.osm
input2=$HOOT_HOME/test-files/DcTigerRoads.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/RubberSheetCmdTest/out.osm
comparefile=$HOOT_HOME/hoot-core-test/src/test/resources/cmd/slow/RubberSheetCmdTest/out.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot rubber-sheet $input1 $input2 $outputfile | \
           sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g" |\
           sed "s/RMSE: [0-9]\+\.[0-9]\+//g"

# Check output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile

