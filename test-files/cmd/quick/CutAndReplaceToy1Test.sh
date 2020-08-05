#!/bin/bash

IN_DIR=test-files/cmd/quick/CutAndReplaceToy1Test
OUT_DIR=test-output/cmd/quick/CutAndReplaceToy1Test

#CONFIG="-C DeriveChangeset.conf -C Testing.conf"
CONFIG="-C DeriveChangeset.conf"

mkdir -p $OUT_DIR

hoot changeset-derive-replacement --info $CONFIG -D bounds.output.file=$OUT_DIR/bounds.osm -D debug.maps.write=false -D debug.maps.filename=$OUT_DIR/debug.osm -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D convert.bounding.box.remove.missing.elements=false $IN_DIR/input1.osm $IN_DIR/input2.osm "-80.06245851785,32.66639316019,-80.05948663026,32.66808435424" $OUT_DIR/output.osc --full-replacement --write-bounds --disable-conflation --disable-cleaning

#diff $CONFIG $IN_DIR/rail_way_snapping_diff_default.osc $OUT_DIR/rail_way_snapping_diff_default.osc
