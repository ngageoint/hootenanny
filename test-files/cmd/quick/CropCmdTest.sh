#!/bin/bash
set -e

IN_DIR=test-files/cmd/quick/CropCmdTest
OUT_DIR=test-output/cmd/quick/CropCmdTest
mkdir -p $OUT_DIR

BOUNDS="-77.05,38.888,-77.03,38.90"
inputfile1=test-files/DcGisRoads.osm
inputfile2=test-files/DcTigerRoads.osm
comparefile1=$IN_DIR/cropped.osm
outputfile1=$OUT_DIR/cropped.osm
comparefile2=$IN_DIR/croppedMultiple.osm
outputfile2=$OUT_DIR/croppedMultiple.osm

RECURSIVE_BOUNDS="-104.9007,38.8540,-104.8994,38.8552"
RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir
GOLD_FILE_RECURSIVE_1=$IN_DIR/cropped-recursive-1.osm
GOLD_FILE_RECURSIVE_2=$IN_DIR/cropped-recursive-2.osm
OUTPUT_FILE_RECURSIVE_1=$OUT_DIR/cropped-recursive-1.osm
OUTPUT_FILE_RECURSIVE_2=$OUT_DIR/cropped-recursive-2.osm

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

echo ""
echo "Cropping recursively in a directory structure..."
echo ""
# Since the files in the dir are duplicated, the output should have multiple copies of the same 
# cropped data.
hoot crop $LOG_LEVEL $CONFIG $RECURSIVE_INPUT $OUTPUT_FILE_RECURSIVE_1 $RECURSIVE_BOUNDS --recursive "*"
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_RECURSIVE_1 $OUTPUT_FILE_RECURSIVE_1 || diff $GOLD_FILE_RECURSIVE_1 $OUTPUT_FILE_RECURSIVE_1

echo ""
echo "Cropping recursively in a directory structure with one filter..."
echo ""
# The files in the dir are duplicated, but the filter only matches one file. So, the output should 
# have a single copy of the cropped data.
hoot crop $LOG_LEVEL $CONFIG $RECURSIVE_INPUT $OUTPUT_FILE_RECURSIVE_2 $RECURSIVE_BOUNDS --recursive "*.json"
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_RECURSIVE_2 $OUTPUT_FILE_RECURSIVE_2 || diff $GOLD_FILE_RECURSIVE_2 $OUTPUT_FILE_RECURSIVE_2

