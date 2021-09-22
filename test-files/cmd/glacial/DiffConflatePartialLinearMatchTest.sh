#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/glacial/DiffConflatePartialLinearMatchTest
INPUT_DIR_2=test-files/cmd/glacial/DiffConflateCmdTest
OUTPUT_DIR=test-output/cmd/glacial/DiffConflatePartialLinearMatchTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf -D match.creators=HighwayMatchCreator -D merger.creators=HighwayMergerCreator -D writer.include.debug.tags=true -D uuid.helper.repeatable=true"

# This test illustrates that diff conflate is capable of not dropping sections from a diff for 
# road features involved in a match with another feature only partially.
 
# Remove partial linear matches partially. This results in a more granular diff.
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false \
  $INPUT_DIR_2/input1.osm $INPUT_DIR_2/input2.osm $OUTPUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $INPUT_DIR/output-partial.osm $OUTPUT_DIR/output-partial.osm
if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUTPUT_DIR/output.osm \
    --report-output $OUTPUT_DIR/output-partial-validation-report --output \
    $OUTPUT_DIR/output-partial-validated.osm
  diff $INPUT_DIR/output-partial-validation-report $OUTPUT_DIR/output-partial-validation-report
fi

# Remove partial linear matches completely. This results in a less granular diff.
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=true \
  $INPUT_DIR_2/input1.osm $INPUT_DIR_2/input2.osm $OUTPUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $INPUT_DIR/output-complete.osm $OUTPUT_DIR/output-complete.osm
if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUTPUT_DIR/output.osm \
    --report-output $OUTPUT_DIR/output-complete-validation-report --output \
    $OUTPUT_DIR/output-complete-validated.osm
  diff $INPUT_DIR/output-complete-validation-report $OUTPUT_DIR/output-complete-validation-report
fi

# Remove partial linear matches partially and remove small secondary fragments from the diff to 
# clean it up a little.
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false \
  -D differential.sec.way.removal.criteria="HighwayCriterion" \
  -D differential.sec.way.removal.length.threshold=15.0 $INPUT_DIR_2/input1.osm \
  $INPUT_DIR_2/input2.osm $OUTPUT_DIR/output-partial-cleaned.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf \
  $INPUT_DIR/output-partial-cleaned.osm $OUTPUT_DIR/output-partial-cleaned.osm
if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUTPUT_DIR/output.osm \
    --report-output $OUTPUT_DIR/output-partial-cleaned-validation-report --output \
    $OUTPUT_DIR/output-partial-cleaned-validated.osm
  diff $INPUT_DIR/output-partial-cleaned-validation-report $OUTPUT_DIR/output-partial-cleaned-validation-report
fi
