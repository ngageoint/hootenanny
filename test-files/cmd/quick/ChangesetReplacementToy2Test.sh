#!/bin/bash

IN_DIR=test-files/cmd/quick/ChangesetReplacementToy2Test
OUT_DIR=test-output/cmd/quick/ChangesetReplacementToy2Test

CONFIG="--warn -C Testing.conf"

mkdir -p $OUT_DIR

# This tests that changesets are never derived with both delete and modify changes for the same element. The modify change takes precedence.

hoot changeset-derive $CONFIG -D bounds="-115.036717,36.04301,-115.035772,36.044142" -D bounds.output.file=$OUT_DIR/bounds.osm -D debug.maps.write=false -D debug.maps.filename=$OUT_DIR/debug.osm -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D bounds.remove.missing.elements=false $IN_DIR/input1.osm $IN_DIR/input2.osm $OUT_DIR/output.osc --write-bounds --replacement

diff $IN_DIR/output.osc $OUT_DIR/output.osc
