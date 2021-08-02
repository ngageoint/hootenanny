#!/bin/bash
set -e

IN_DIR=test-files/ops/JosmMapValidatorTest
OUTPUT_DIR=test-output/cmd/slow/JosmValidateCmdTest
mkdir -p $OUTPUT_DIR

LOG_LEVEL=--warn
CONFIG="-C Testing.conf"

VALIDATORS="UntaggedWay;UnclosedWays;DuplicatedWayNodes"

inputfile=test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm
comparefile=$IN_DIR/runValidateTest-out.osm
outputfile=$OUTPUT_DIR/out.osm
COMPARE_FILE_MULTIPLE=$IN_DIR/runValidateTest-multiple-out.osm
OUTPUT_FILE_MULTIPLE=$OUTPUT_DIR/out-multiple.osm

SEPARATE_OUTPUT_INPUT=$OUTPUT_DIR/runCleanTest-in.osm
SEPARATE_OUTPUT_OUTPUT=$OUTPUT_DIR/runCleanTest-in-validated.osm

echo ""
echo "Listing available validators..."
echo ""
hoot validate $LOG_LEVEL $CONFIG --available-validators | grep "DuplicatedWayNodes"

echo ""
echo "Validating a single input..."
echo ""
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.include=$VALIDATORS $inputfile

echo ""
echo "Validating a single input and using an output file..."
echo ""
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.include=$VALIDATORS $inputfile --output $outputfile
hoot diff $LOG_LEVEL $CONFIG $comparefile $outputfile || diff $comparefile $outputfile

echo ""
echo "Validating multiple inputs..."
echo ""
# Just loading the same file twice for now...could eventually get a second input file. Should get 
# double the number of validation errors compared to the single input file.
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.include=$VALIDATORS $inputfile $inputfile --output $OUTPUT_FILE_MULTIPLE
hoot diff $LOG_LEVEL $CONFIG $COMPARE_FILE_MULTIPLE $OUTPUT_FILE_MULTIPLE || diff $COMPARE_FILE_MULTIPLE $OUTPUT_FILE_MULTIPLE

echo ""
echo "Validating recursively in a directory structure with a filter..."
echo ""
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.include=$VALIDATORS test-files/ops/JosmMapCleanerTest --recursive "*.json"

echo ""
echo "Validating files and writing to separate outputs..."
echo ""
# Copy the inputs to the output directory and work from there, so we don't write to the input dir 
# under source control.
cp $inputfile $OUTPUT_DIR
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.include=$VALIDATORS $SEPARATE_OUTPUT_INPUT --separate-output
hoot diff $LOG_LEVEL $CONFIG $comparefile $SEPARATE_OUTPUT_OUTPUT || diff $comparefile $SEPARATE_OUTPUT_OUTPUT
