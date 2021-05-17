#!/bin/bash
set -e

# This tests to see if collection relations (e.g. admin bounds, routes, etc.) get merged correctly during conflation.
# Unfortunately the input is very large for this test, but it took input of that size to reproduce the original problem. When
# examining changes in output, focus on the relations primarily. Changes to non-relation features aren't that important to 
# this test and are tested thoroughly elsewhere.

IN_DIR=test-files/cmd/glacial/RelationMergeTest
OUT_DIR=test-output/cmd/glacial/RelationMergeTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -D "writer.include.debug.tags=true" -D "uuid.helper.repeatable=true" $IN_DIR/input1.osm $IN_DIR/input2.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
