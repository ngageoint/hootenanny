#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/quick/JosmValidateCmdTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

inputfile=test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm
comparefile=test-files/ops/JosmMapValidatorTest/runValidateTest-out.osm
outputfile=$OUTPUT_DIR/out.osm

# list available validators
hoot validate --available-validators | grep "DuplicatedWayNodes"

# validation
hoot validate $LOG_LEVEL -C Testing.conf -D josm.validators.include="UntaggedWay;UnclosedWays;DuplicatedWayNodes" $inputfile $outputfile
hoot diff $LOG_LEVEL $comparefile $outputfile || diff $comparefile $outputfile
