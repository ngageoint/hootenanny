#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/JosmValidateCmdTest
OUTPUT_DIR=test-output/cmd/slow/JosmValidateCmdTest
mkdir -p $OUTPUT_DIR

LOG_LEVEL=--warn
CONFIG="-C Testing.conf"

INPUT=test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm

OUTPUT_SINGLE=$OUTPUT_DIR/single-out.osm
GOLD_SINGLE=$IN_DIR/single-out.osm

OUTPUT_FILE_MULTIPLE=$OUTPUT_DIR/multiple-out.osm
GOLD_MULTIPLE=$IN_DIR/multiple-out.osm

SEPARATE_OUTPUT_OUTPUT=$OUTPUT_DIR/separate-out.osm
SEPARATE_OUTPUT_GOLD=$IN_DIR/separate-out.osm

echo ""
echo "Listing available validators..."
echo ""
hoot validate $LOG_LEVEL $CONFIG --validators | grep "DuplicatedWayNodes"

echo ""
echo "Validating a single input..."
echo ""
# No output being compared here, just making sure it runs.
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT

echo ""
echo "Validating a single input and using an output file..."
echo ""
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT --output $OUTPUT_SINGLE
hoot diff $LOG_LEVEL $CONFIG $GOLD_SINGLE $OUTPUT_SINGLE || diff $GOLD_SINGLE $OUTPUT_SINGLE

echo ""
echo "Validating multiple inputs..."
echo ""
# Just loading the same file twice for now...could eventually get a second input file. Should get 
# double the number of validation errors compared to the single input file.
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT $INPUT --output $OUTPUT_FILE_MULTIPLE
hoot diff $LOG_LEVEL $CONFIG $GOLD_MULTIPLE $OUTPUT_FILE_MULTIPLE || diff $GOLD_MULTIPLE $OUTPUT_FILE_MULTIPLE

echo ""
echo "Validating recursively in a directory structure with a filter..."
echo ""
# TODO: need to compare some output here
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators test-files/ops/JosmMapCleanerTest --recursive "*.json"

echo ""
echo "Validating files and writing to separate outputs..."
echo ""
# Copy the inputs to the output directory and work from there, so we don't write to the input dir 
# under source control.
cp $INPUT $OUTPUT_DIR
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT --separate-output
# --separate-output keeps the input file names, but let's rename this to something more recognizable.
mv $OUTPUT_DIR/runCleanTest-in.osm $SEPARATE_OUTPUT_OUTPUT
hoot diff $LOG_LEVEL $CONFIG $SEPARATE_OUTPUT_GOLD $SEPARATE_OUTPUT_OUTPUT || diff $SEPARATE_OUTPUT_GOLD $SEPARATE_OUTPUT_OUTPUT
