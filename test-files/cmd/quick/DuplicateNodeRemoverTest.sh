#!/bin/bash

IN_DIR=test-files/cmd/quick/DuplicateNodeRemoverTest
OUT_DIR=test-output/cmd/quick/DuplicateNodeRemoverTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot convert $CONFIG -D convert.ops="DuplicateNodeRemover" $IN_DIR/input.json $OUT_DIR/output.json
hoot diff $CONFIG $IN_DIR/input.json $OUT_DIR/output.json || diff $IN_DIR/input.json $OUT_DIR/output.json

hoot convert $CONFIG -D convert.ops="DuplicateNodeRemover" $IN_DIR/crossing_test.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/crossing_input.osm $OUT_DIR/output.osm || diff $IN_DIR/crossing_input.osm $OUT_DIR/output.osm
