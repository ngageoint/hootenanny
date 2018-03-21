#!/bin/bash
set -e

export INPUTS="test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm"
export OUTPUT_DIR=test-output/cmd/ConflateConsistencyTest
mkdir -p test-output/cmd/ConflateConsistencyTest
hoot conflate -D uuid.helper.repeatable=true $INPUTS \
    $OUTPUT_DIR/Output1.osm
hoot conflate -D uuid.helper.repeatable=true $INPUTS \
    $OUTPUT_DIR/Output2.osm

diff --brief $OUTPUT_DIR/Output1.osm $OUTPUT_DIR/Output2.osm
