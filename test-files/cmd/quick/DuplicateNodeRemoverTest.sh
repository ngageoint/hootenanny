#!/bin/bash

IN_DIR=test-files/cmd/quick/DuplicateNodeRemoverTest
OUT_DIR=test-output/cmd/quick/DuplicateNodeRemoverTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot convert $CONFIG -D convert.ops="hoot::DuplicateNodeRemover" $IN_DIR/input.json $OUT_DIR/output.json
hoot diff $CONFIG $IN_DIR/input.json $OUT_DIR/output.json || diff $IN_DIR/input.json $OUT_DIR/output.json
