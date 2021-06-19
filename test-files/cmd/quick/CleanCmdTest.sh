#!/bin/bash
set -e

IN_DIR=test-files/cmd/quick/CleanCmdTest
OUT_DIR=test-output/cmd/quick/CleanCmdTest
mkdir -p $OUT_DIR

inputfile=test-files/DcGisRoads.osm
comparefile=$IN_DIR/cleaned.osm
outputfile=$OUT_DIR/cleaned.osm

GOLD_FILE_MULTIPLE=$IN_DIR/cleaned-multiple.osm
INPUT_MULTIPLE="test-files/DcGisRoads.osm test-files/DcTigerRoads.osm"
OUTPUT_FILE_MULTIPLE=$OUT_DIR/cleaned-multiple.osm

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
