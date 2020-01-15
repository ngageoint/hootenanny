#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/slow/ConflateGenericCmdTest
OUTPUT_DIR=test-output/cmd/slow/ConflateGenericCmdTest
mkdir -p $OUTPUT_DIR

export CONFLATE_OPTS="-C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D conflate.match.unmatched.with.generic=true -D writer.include.debug.tags=true"

# TODO: move to case test
hoot conflate $CONFLATE_OPTS $INPUT_DIR/ConflateGenericCmdTestA.osm $INPUT_DIR/ConflateGenericCmdTestB.osm $OUTPUT_DIR/out.osm
hoot diff $OUTPUT_DIR/out.osm $INPUT_DIR/out.osm || diff $OUTPUT_DIR/out.osm $INPUT_DIR/out.osm
