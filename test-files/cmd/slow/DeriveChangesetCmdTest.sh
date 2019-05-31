#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/DeriveChangesetCmdTest
OUTPUT_DIR=test-output/cmd/slow/DeriveChangesetCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

# IN MEMORY SORTING (default)

hoot changeset-derive -C Testing.conf $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-1.osc
diff $INPUT_DIR/changeset-1.osc $OUTPUT_DIR/changeset-1.osc 

hoot changeset-derive -C Testing.conf $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-2.osc
diff $INPUT_DIR/changeset-2.osc $OUTPUT_DIR/changeset-2.osc 

# Add at least one map consumer in the convert ops to force in memory sorting when no element buffer size is specified.
hoot changeset-derive -C Testing.conf -D convert.ops="hoot::MapCropper;hoot::SetTagValueVisitor" -D crop.bounds="104.8948,38.8519,104.9003,38.85545" -D set.tag.value.visitor.key=test_key -D set.tag.value.visitor.value=test_val $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-3.osc
diff $INPUT_DIR/changeset-3.osc $OUTPUT_DIR/changeset-3.osc 

# A set of all streamable convert ops should work with in memory sorting as well.
hoot changeset-derive -C Testing.conf -D convert.ops="hoot::SetTagValueVisitor;hoot::ReplaceTagVisitor" -D set.tag.value.visitor.key=test_key -D set.tag.value.visitor.value=test_val -D replace.tag.visitor.match.tag="highway=road" -D replace.tag.visitor.replace.tag="highway=primary" $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-4.osc
diff $INPUT_DIR/changeset-4.osc $OUTPUT_DIR/changeset-4.osc 

# EXTERNAL MERGE SORTING

hoot changeset-derive -C Testing.conf -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-5.osc
diff $INPUT_DIR/changeset-1.osc $OUTPUT_DIR/changeset-5.osc 

hoot changeset-derive -C Testing.conf -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-6.osc
diff $INPUT_DIR/changeset-2.osc $OUTPUT_DIR/changeset-6.osc 

# If all convert ops are streamable (not map consumers), then external merge sorting will occur if a proper element buffer size was specified.
hoot changeset-derive -C Testing.conf -D element.sorter.element.buffer.size=5 -D convert.ops="hoot::SetTagValueVisitor;hoot::ReplaceTagVisitor" -D set.tag.value.visitor.key=test_key -D set.tag.value.visitor.value=test_val -D replace.tag.visitor.match.tag="highway=road" -D replace.tag.visitor.replace.tag="highway=primary" $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-7.osc
diff $INPUT_DIR/changeset-4.osc $OUTPUT_DIR/changeset-7.osc 
