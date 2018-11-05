#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/DeriveChangesetCmdTest
OUTPUT_DIR=test-output/cmd/DeriveChangesetCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

# in memory sorting tests

hoot changeset-derive $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-1.osc
diff $INPUT_DIR/changeset-1.osc $OUTPUT_DIR/changeset-1.osc 

hoot changeset-derive $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-2.osc
diff $INPUT_DIR/changeset-2.osc $OUTPUT_DIR/changeset-2.osc 

# external merge sorting tests

hoot changeset-derive -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-3.osc
diff $INPUT_DIR/changeset-1.osc $OUTPUT_DIR/changeset-3.osc 

hoot changeset-derive -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-4.osc
diff $INPUT_DIR/changeset-2.osc $OUTPUT_DIR/changeset-4.osc 
