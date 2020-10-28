#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/RndPlotNodeDensityCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.pbf
outputfile=$HOOT_HOME/test-output/cmd/glacial/RndPlotNodeDensityCmdTest/out.png

CONFIG="-C Testing.conf"

hoot plot-node-density $CONFIG $inputfile $outputfile 1024 

# Compare to known-good
goodfile=$HOOT_HOME/test-files/cmd/glacial/RndPlotNodeDensityCmdTest/out.png
cmp $goodfile $outputfile
if [ "$?" = "1" ]; then
  echo "PNG files are not equal! Compare $goodfile $testfile visually"
  exit 1
fi

