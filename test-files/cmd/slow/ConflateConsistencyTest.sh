#!/bin/bash
set -e

HOOT_OPTS="-D hash.seed.zero=true -D uuid.helper.repeatable=true"

export INPUTS="test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm"
export OUTPUT_DIR=test-output/cmd/ConflateConsistencyTest
mkdir -p test-output/cmd/ConflateConsistencyTest
hoot conflate $HOOT_OPTS $INPUTS \
    $OUTPUT_DIR/Output1.osm
hoot conflate $HOOT_OPTS $INPUTS \
    $OUTPUT_DIR/Output2.osm

diff --brief $OUTPUT_DIR/Output1.osm $OUTPUT_DIR/Output2.osm
