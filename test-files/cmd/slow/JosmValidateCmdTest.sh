#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/JosmValidateCmdTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

CONFIG="-C Testing.conf"

inputfile=test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm
comparefile=test-files/ops/JosmMapValidatorTest/runValidateTest-out.osm
outputfile=$OUTPUT_DIR/out.osm

# list available validators
hoot validate $CONFIG --available-validators | grep "DuplicatedWayNodes"

# validation
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.include="UntaggedWay;UnclosedWays;DuplicatedWayNodes" $inputfile $outputfile
hoot diff $LOG_LEVEL $CONFIG $comparefile $outputfile || diff $comparefile $outputfile
