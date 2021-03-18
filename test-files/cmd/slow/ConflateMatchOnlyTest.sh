#!/bin/bash
set -e

INPUT_DIR=test-files/conflate/unified
GOLD_DIR=test-files/cmd/slow/ConflateMatchOnlyTest
OUTPUT_DIR=test-output/cmd/slow/ConflateMatchOnlyTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL="--warn"
CONFIG="-C Testing.conf -D uuid.helper.repeatable=true -D conflate.match.only=true -D writer.include.conflate.score.tags=true"

hoot conflate $LOG_LEVEL $CONFIG $INPUT_DIR/AllDataTypesA.osm $INPUT_DIR/AllDataTypesB.osm $OUTPUT_DIR/out.osm
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/out.osm $GOLD_DIR/out.osm || diff $OUTPUT_DIR/out.osm $GOLD_DIR/out.osm
