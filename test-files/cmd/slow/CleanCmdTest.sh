#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/CleanCmdTest
OUT_DIR=test-output/cmd/slow/CleanCmdTest
mkdir -p $OUT_DIR

inputfile=test-files/DcGisRoads.osm
comparefile=$IN_DIR/cleaned.osm
outputfile=$OUT_DIR/cleaned.osm

GOLD_FILE_MULTIPLE=$IN_DIR/cleaned-multiple.osm
INPUT_MULTIPLE="test-files/DcGisRoads.osm test-files/DcTigerRoads.osm"
OUTPUT_FILE_MULTIPLE=$OUT_DIR/cleaned-multiple.osm

RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir
GOLD_FILE_RECURSIVE_1=$IN_DIR/cleaned-recursive-1.osm
GOLD_FILE_RECURSIVE_2=$IN_DIR/cleaned-recursive-2.osm
OUTPUT_FILE_RECURSIVE_1=$OUT_DIR/cleaned-recursive-1.osm
OUTPUT_FILE_RECURSIVE_2=$OUT_DIR/cleaned-recursive-2.osm

SEPARATE_OUTPUT_INPUT_1=$OUT_DIR/DcGisRoads.osm
SEPARATE_OUTPUT_INPUT_2=$OUT_DIR/DcTigerRoads.osm
SEPARATE_OUTPUT_OUTPUT_1=$OUT_DIR/DcGisRoads-cleaned.osm
SEPARATE_OUTPUT_OUTPUT_2=$OUT_DIR/DcTigerRoads-cleaned.osm
GOLD_FILE_SEPARATE_OUTPUT_1=$comparefile
GOLD_FILE_SEPARATE_OUTPUT_2=$IN_DIR/DcTigerRoads-cleaned.osm

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Cleaning a single input..."
echo ""
hoot clean $LOG_LEVEL $CONFIG $inputfile $outputfile
hoot diff $LOG_LEVEL $CONFIG $comparefile $outputfile || diff $comparefile $outputfile

echo ""
echo "Cleaning multiple inputs..."
echo ""
hoot clean $LOG_LEVEL $CONFIG $INPUT_MULTIPLE $OUTPUT_FILE_MULTIPLE
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_MULTIPLE $OUTPUT_FILE_MULTIPLE || diff $GOLD_FILE_MULTIPLE $OUTPUT_FILE_MULTIPLE

echo ""
echo "Cleaning recursively in a directory structure..."
echo ""
# Since the data in the dir is duplicated and cleaning removes duplicate features, the output here is just that of
# a single input.
hoot clean $LOG_LEVEL $CONFIG $RECURSIVE_INPUT $OUTPUT_FILE_RECURSIVE_1 --recursive "*"
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_RECURSIVE_1 $OUTPUT_FILE_RECURSIVE_1 || diff $GOLD_FILE_RECURSIVE_1 $OUTPUT_FILE_RECURSIVE_1

echo ""
echo "Cleaning one file and also recursively in a directory structure with one filter..."
echo ""
# The inputs here are from two fairly far apart locations, but all you really need to check is that both
# areas are in the output.
hoot clean $LOG_LEVEL $CONFIG $inputfile $RECURSIVE_INPUT $OUTPUT_FILE_RECURSIVE_2 --recursive "*.osm"
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_RECURSIVE_2 $OUTPUT_FILE_RECURSIVE_2 || diff $GOLD_FILE_RECURSIVE_2 $OUTPUT_FILE_RECURSIVE_2

echo ""
echo "Cleaning files and writing to separate outputs..."
echo ""
# TODO
cp $INPUT_MULTIPLE $OUT_DIR
hoot clean $LOG_LEVEL $CONFIG $SEPARATE_OUTPUT_INPUT_1 $SEPARATE_OUTPUT_INPUT_2 --separate-output
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_SEPARATE_OUTPUT_1 $SEPARATE_OUTPUT_OUTPUT_1 || diff $GOLD_FILE_SEPARATE_OUTPUT_1 $SEPARATE_OUTPUT_OUTPUT_1
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_SEPARATE_OUTPUT_2 $SEPARATE_OUTPUT_OUTPUT_2 || diff $GOLD_FILE_SEPARATE_OUTPUT_2 $SEPARATE_OUTPUT_OUTPUT_2
