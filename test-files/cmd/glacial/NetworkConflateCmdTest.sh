#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/glacial/NetworkConflateCmdTest
OUTPUT_DIR=test-output/cmd/glacial/NetworkConflateCmdTest
mkdir -p $OUTPUT_DIR

HOOT_OPTS="-D uuid.helper.repeatable=true -D writer.include.debug.tags=true -D match.creators=NetworkMatchCreator -D merger.creators=NetworkMergerCreator"
source scripts/core/ScriptTestUtils.sh

hoot conflate --warn -C ReferenceConflation.conf -C NetworkAlgorithm.conf -C Testing.conf  \
  $HOOT_OPTS test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUTPUT_DIR/output.osm
hoot diff -C Testing.conf $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm || \
  diff $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm
validateTestOutput $OUTPUT_DIR/output.osm $OUTPUT_DIR/output-validation-report \
  $OUTPUT_DIR/output-validated.osm $INPUT_DIR/output-validation-report

# Check to make sure we don't bomb out on empty files
hoot conflate --warn -C ReferenceConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 $HOOT_OPTS test-files/Empty.osm test-files/Empty.osm tmp/NetworkConflateCmdTest.osm
hoot diff -C Testing.conf test-files/Empty.osm tmp/NetworkConflateCmdTest.osm || cat tmp/NetworkConflateCmdTest.osm
