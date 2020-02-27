#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/SortCmdTest
OUTPUT_DIR=test-output/cmd/slow/SortCmdTest
mkdir -p $OUTPUT_DIR

BUFFER_SIZE=3

CONFIG="-C Testing.conf"

# There is no geonames writer, so we're writing that input to osm xml.

# in memory sorting tests

echo "Sorting geonames in memory..."
echo ""
hoot is-sorted $CONFIG $INPUT_DIR/input.geonames
hoot sort --warn $CONFIG $INPUT_DIR/input.geonames $OUTPUT_DIR/geonames-output-1.osm
hoot is-sorted $CONFIG $OUTPUT_DIR/geonames-output-1.osm
hoot diff --warn $CONFIG $INPUT_DIR/geonames-output.osm $OUTPUT_DIR/geonames-output-1.osm
echo ""

echo "Sorting xml in memory..."
echo ""
hoot is-sorted $CONFIG $INPUT_DIR/input.osm
hoot sort --warn $CONFIG $INPUT_DIR/input.osm $OUTPUT_DIR/osm-output-1.osm
hoot is-sorted $CONFIG $OUTPUT_DIR/osm-output-1.osm
hoot diff --warn $CONFIG $INPUT_DIR/osm-output.osm $OUTPUT_DIR/osm-output-1.osm
echo ""

echo "Sorting pbf in memory..."
echo ""
hoot is-sorted $CONFIG $INPUT_DIR/input.osm.pbf
hoot sort --warn $CONFIG $INPUT_DIR/input.osm.pbf $OUTPUT_DIR/pbf-output-1.osm.pbf
hoot is-sorted $CONFIG $OUTPUT_DIR/pbf-output-1.osm.pbf
hoot diff --warn $CONFIG $INPUT_DIR/pbf-output.osm.pbf $OUTPUT_DIR/pbf-output-1.osm.pbf
echo ""

# external merge sorting tests

# The element not found warnings are expected here, as during the external sorting the temp files may have partial maps in them.  We just
# need to be sure the final output matches what is expected

echo "Sorting geonames on disk..."
echo ""
hoot is-sorted $CONFIG $INPUT_DIR/input.geonames
hoot sort --warn $CONFIG -D element.sorter.element.buffer.size=$BUFFER_SIZE $INPUT_DIR/input.geonames $OUTPUT_DIR/geonames-output-2.osm
hoot is-sorted $CONFIG $OUTPUT_DIR/geonames-output-2.osm
hoot diff --warn $CONFIG $INPUT_DIR/geonames-output.osm $OUTPUT_DIR/geonames-output-2.osm
echo ""

echo "Sorting xml on disk..."
echo ""
hoot is-sorted $CONFIG $INPUT_DIR/input.osm
hoot sort --warn $CONFIG -D element.sorter.element.buffer.size=$BUFFER_SIZE $INPUT_DIR/input.osm $OUTPUT_DIR/osm-output-2.osm
hoot is-sorted $CONFIG $OUTPUT_DIR/osm-output-2.osm
hoot diff --warn $CONFIG $INPUT_DIR/osm-output.osm $OUTPUT_DIR/osm-output-2.osm
echo ""

echo "Sorting pbf on disk..."
echo ""
hoot is-sorted $CONFIG $INPUT_DIR/input.osm.pbf
hoot sort --warn $CONFIG -D element.sorter.element.buffer.size=$BUFFER_SIZE $INPUT_DIR/input.osm.pbf $OUTPUT_DIR/pbf-output-2.osm.pbf
hoot is-sorted $CONFIG $OUTPUT_DIR/pbf-output-2.osm.pbf
hoot diff --warn $CONFIG $INPUT_DIR/pbf-output.osm.pbf $OUTPUT_DIR/pbf-output-2.osm.pbf
echo ""
