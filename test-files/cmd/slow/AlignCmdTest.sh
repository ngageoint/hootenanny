#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/AlignCmdTest

input1=$HOOT_HOME/test-files/DcGisRoads.osm
input2=$HOOT_HOME/test-files/DcTigerRoads.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/AlignCmdTest/out.osm
comparefile=$HOOT_HOME/test-files/cmd/slow/AlignCmdTest/out.osm

CONFIG="-C Testing.conf"

# Run the command.
hoot align $CONFIG $input1 $input2 $outputfile | \
           sed "s/RMSE: [0-9]\+\.[0-9]\+//g"

# Check output
hoot diff $CONFIG $comparefile $outputfile || diff $comparefile $outputfile

