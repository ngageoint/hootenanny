#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/TileConflateCmdTest

input1=$HOOT_HOME/test-files/LeftTile.osm
input2=$HOOT_HOME/test-files/RightTile.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/CropMapCmdTest/conflated.osm
comparefile=$HOOT_HOME/hoot-core-test/src/test/resources/cmd/slow/TileConflateCmdTest/conflated.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot tile-conflate $input1 $input2 $outputfile | \
           sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g"

# Check output
#hoot is-match $comparefile $outputfile || diff $comparefile $outputfile

