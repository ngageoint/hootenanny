#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/NodeDensityPlotCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.pbf
outputfile=$HOOT_HOME/test-output/cmd/quick/NodeDensityPlotCmdTest/out.png

CONFIG="-C Testing.conf"

hoot node-density-plot $CONFIG $inputfile $outputfile 1024 

# Compare to known-good
goodfile=$HOOT_HOME/test-files/cmd/quick/NodeDensityPlotCmdTest/out.png
cmp $goodfile $outputfile
if [ "$?" = "1" ]; then
  echo "PNG files are not equal! Compare $goodfile $testfile visually"
  exit 1
fi

