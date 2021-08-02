#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/glacial/DiffConflatePartialLinearMatchTest
OUTPUT_DIR=test-output/cmd/glacial/DiffConflatePartialLinearMatchTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf -D match.creators=hoot::HighwayMatchCreator -D merger.creators=hoot::HighwayMergerCreator -D writer.include.debug.tags=true -D uuid.helper.repeatable=true"

# This test illustrates that diff conflate is capable of not dropping sections from a diff for 
# road features involved in a match with another feature only partially. The test is run two times, 
# once removing partial matches partially and once removing partial matches completely to show the 
# difference.
 
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $INPUT_DIR/output-partial.osm $OUTPUT_DIR/output-partial.osm

hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=true $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $INPUT_DIR/output-complete.osm $OUTPUT_DIR/output-complete.osm
