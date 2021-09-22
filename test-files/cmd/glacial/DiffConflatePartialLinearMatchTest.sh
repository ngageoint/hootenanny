#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/glacial/DiffConflatePartialLinearMatchTest
OUTPUT_DIR=test-output/cmd/glacial/DiffConflatePartialLinearMatchTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf -D match.creators=HighwayMatchCreator -D merger.creators=HighwayMergerCreator -D writer.include.debug.tags=true -D uuid.helper.repeatable=true"

# This test illustrates that diff conflate is capable of not dropping sections from a diff for 
# road features involved in a match with another feature only partially.
 
# Remove partial linear matches partially. This results in a more granular diff.
TO_VALIDATE_1=test-output/cmd/glacial/DiffConflatePartialLinearMatchTest/output-partial.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/glacial/DiffConflatePartialLinearMatchTest/output-partial-validation-report
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false \
  $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $INPUT_DIR/output-partial.osm $OUTPUT_DIR/output-partial.osm

# Remove partial linear matches completely. This results in a less granular diff.
TO_VALIDATE_2=test-output/cmd/glacial/DiffConflatePartialLinearMatchTest/output-complete.osm
VALIDATION_REPORT_GOLD_2=test-files/cmd/glacial/DiffConflatePartialLinearMatchTest/output-complete-validation-report
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=true \
  $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $INPUT_DIR/output-complete.osm $OUTPUT_DIR/output-complete.osm

# Remove partial linear matches partially and remove small secondary fragments from the diff to 
# clean it up a little.
TO_VALIDATE_3=test-output/cmd/glacial/DiffConflatePartialLinearMatchTest/output-partial-cleaned.osm
VALIDATION_REPORT_GOLD_3=test-files/cmd/glacial/DiffConflatePartialLinearMatchTest/output-partial-cleaned-validation-report
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false \
  -D differential.sec.way.removal.criteria="HighwayCriterion" \
  -D differential.sec.way.removal.length.threshold=15.0 $INPUT_DIR/input1.osm \
  $INPUT_DIR/input2.osm $OUTPUT_DIR/output-partial-cleaned.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf \
  $INPUT_DIR/output-partial-cleaned.osm $OUTPUT_DIR/output-partial-cleaned.osm
