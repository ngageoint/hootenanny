#!/bin/bash
set -e

mkdir -p test-output/cmd/quick/CropCmdTest

BOUNDS="-77.05,38.888,-77.03,38.90 "
inputfile1=test-files/DcGisRoads.osm
inputfile2=test-files/DcTigerRoads.osm
comparefile1=test-files/cmd/quick/CropCmdTest/cropped.osm
outputfile1=test-output/cmd/quick/CropCmdTest/cropped.osm
comparefile2=test-files/cmd/quick/CropCmdTest/croppedMultiple.osm
outputfile2=test-output/cmd/quick/CropCmdTest/croppedMultiple.osm

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Cropping single input..."
echo ""
hoot crop $LOG_LEVEL $CONFIG $inputfile1 $outputfile1 $BOUNDS
hoot diff $LOG_LEVEL $CONFIG $comparefile1 $outputfile1 || diff $comparefile1 $outputfile1

echo ""
echo "Cropping multiple inputs into a single output..."
echo ""
hoot crop $LOG_LEVEL $CONFIG $inputfile1 $inputfile2 $outputfile2 $BOUNDS
hoot diff $LOG_LEVEL $CONFIG $comparefile2 $outputfile2 || diff $comparefile2 $outputfile2
