#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/slow/DiffConflatePartialLinearMatchTest
OUTPUT_DIR=test-output/cmd/slow/DiffConflatePartialLinearMatchTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

# This test illustrates that diff conflate is capable of not dropping sections from a diff for 
# features involved in a match with another feature only partially. If 
# differential.remove.linear.partial.matches.as.whole is temporarily enabled, you can see the difference
# as several roads will drop out of the diff.
 
hoot conflate $LOG_LEVEL -C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf -D match.creators="hoot::HighwayMatchCreator" -D merger.creators="hoot::HighwayMergerCreator" -D differential.remove.linear.partial.matches.as.whole=false -D writer.include.debug.tags=true -D uuid.helper.repeatable=true $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/output.osm --differential

hoot diff $LOG_LEVEL -C Testing.conf $INPUT_DIR/output.osm $OUTPUT_DIR/output.osm
