#!/bin/bash

IN_DIR=test-files/cmd/quick/ChangesetReplacementToy1Test
OUT_DIR=test-output/cmd/quick/ChangesetReplacementToy1Test

CONFIG="--warn -C DeriveChangeset.conf -C Testing.conf"

mkdir -p $OUT_DIR

# -D changeset.replacement.implementation=hoot::ChangesetReplacementCreator1
hoot changeset-derive-replacement $CONFIG -D changeset.replacement.implementation=hoot::ChangesetReplacementCreator7 -D bounds.output.file=$OUT_DIR/bounds.osm -D debug.maps.write=false -D debug.maps.filename=$OUT_DIR/debug.osm -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D convert.bounding.box.remove.missing.elements=false $IN_DIR/input1.osm $IN_DIR/input2.osm "-80.06245851785,32.66639316019,-80.05948663026,32.66808435424" $OUT_DIR/output.osc --full-replacement --write-bounds

diff $IN_DIR/output.osc $OUT_DIR/output.osc
