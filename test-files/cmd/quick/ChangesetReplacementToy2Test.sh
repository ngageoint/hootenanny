#!/bin/bash

IN_DIR=test-files/cmd/quick/ChangesetReplacementToy2Test
OUT_DIR=test-output/cmd/quick/ChangesetReplacementToy2Test

CONFIG="--warn -C DeriveChangeset.conf -C Testing.conf"

mkdir -p $OUT_DIR

# This tests that changesets are never derived with both delete and modify changes for the same element. The modify change takes precedence.

hoot changeset-derive-replacement $CONFIG -D bounds.output.file=$OUT_DIR/bounds.osm -D debug.maps.write=false -D debug.maps.filename=$OUT_DIR/debug.osm -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D convert.bounds.remove.missing.elements=false $IN_DIR/input1.osm $IN_DIR/input2.osm "-115.036717,36.04301,-115.035772,36.044142" $OUT_DIR/output.osc --write-bounds

diff $IN_DIR/output.osc $OUT_DIR/output.osc
