#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ChangesetDeriveCmdTest
OUTPUT_DIR=test-output/cmd/slow/ChangesetDeriveCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

# IN MEMORY SORTING (default)

echo "Deriving changeset with in-memory element sorting..."
hoot changeset-derive -C Testing.conf $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-1.osc
diff $INPUT_DIR/changeset-1.osc $OUTPUT_DIR/changeset-1.osc 

echo "Deriving changeset single input with in-memory element sorting..."
hoot changeset-derive -C Testing.conf $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-2.osc
diff $INPUT_DIR/changeset-2.osc $OUTPUT_DIR/changeset-2.osc 

# Add at least one map consumer in the convert ops to force in memory sorting when no element buffer size is specified.
echo "Deriving changeset with unstreamable op..."
hoot changeset-derive -C Testing.conf -D convert.ops="hoot::MapCropper;hoot::SetTagValueVisitor" -D crop.bounds="104.8948,38.8519,104.9003,38.85545" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-3.osc
diff $INPUT_DIR/changeset-3.osc $OUTPUT_DIR/changeset-3.osc 

# Unlike the final error checking test there are no overlapping element IDs in these input files, so the map ops should be applied.
echo "Deriving changeset unstreamable op and no overlapping element IDs..."
hoot changeset-derive -C Testing.conf -D convert.ops="hoot::MapCropper;hoot::SetTagValueVisitor" -D crop.bounds="-104.8055,39.5906,-104.8035,39.594" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val test-files/ToyBuildingsTestA.osm test-files/ToyBuildingsTestB.osm $OUTPUT_DIR/changeset-10.osc
diff $INPUT_DIR/changeset-10.osc $OUTPUT_DIR/changeset-10.osc 

# There are overlapping element IDs between these two files, but since the elements with overlapping IDs are identical in the two files, no
# error should occur.
# TODO: fix
#echo "Deriving changeset with unstreamable op and overlapping element IDs with identical elements..."
#hoot changeset-derive -C Testing.conf -D convert.ops="hoot::MapCropper;hoot::SetTagValueVisitor" -D crop.bounds="104.8948,38.8519,104.9003,38.85545" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-12.osc
#diff $INPUT_DIR/changeset-12.osc $OUTPUT_DIR/changeset-12.osc 

# A set of all streamable convert ops should work with in memory sorting as well.

echo "Deriving changeset with streamable ops..."
hoot changeset-derive -C Testing.conf -D convert.ops="hoot::SetTagValueVisitor;hoot::ReplaceTagVisitor" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val -D replace.tag.visitor.match.tag="highway=road" -D replace.tag.visitor.replace.tag="highway=primary" $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-4.osc
diff $INPUT_DIR/changeset-4.osc $OUTPUT_DIR/changeset-4.osc 

echo "Deriving changeset single input with streamable ops..."
hoot changeset-derive -C Testing.conf -D convert.ops="hoot::SetTagValueVisitor;hoot::ReplaceTagVisitor" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val -D replace.tag.visitor.match.tag="highway=road" -D replace.tag.visitor.replace.tag="highway=primary" $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-8.osc
diff $INPUT_DIR/changeset-8.osc $OUTPUT_DIR/changeset-8.osc 

# EXTERNAL MERGE SORTING

echo "Deriving changeset with external sort buffer specified..."
hoot changeset-derive -C Testing.conf -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-5.osc
diff $INPUT_DIR/changeset-1.osc $OUTPUT_DIR/changeset-5.osc 

echo "Deriving changeset single input with external sort buffer specified..."
hoot changeset-derive -C Testing.conf -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-6.osc
diff $INPUT_DIR/changeset-2.osc $OUTPUT_DIR/changeset-6.osc 

# If all convert ops are streamable (not map consumers), then external merge sorting will occur if a positive element buffer size was specified.

echo "Deriving changeset with streamable ops and external sort buffer specified..."
hoot changeset-derive -C Testing.conf -D element.sorter.element.buffer.size=5 -D convert.ops="hoot::SetTagValueVisitor;hoot::ReplaceTagVisitor" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val -D replace.tag.visitor.match.tag="highway=road" -D replace.tag.visitor.replace.tag="highway=primary" $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-7.osc
diff $INPUT_DIR/changeset-4.osc $OUTPUT_DIR/changeset-7.osc 

echo "Deriving changeset single input with streamable ops and external sort buffer specified..."
hoot changeset-derive -C Testing.conf -D element.sorter.element.buffer.size=5 -D convert.ops="hoot::SetTagValueVisitor;hoot::ReplaceTagVisitor" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val -D replace.tag.visitor.match.tag="highway=road" -D replace.tag.visitor.replace.tag="highway=primary" $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-9.osc
diff $INPUT_DIR/changeset-9.osc $OUTPUT_DIR/changeset-9.osc 

###################################

# IMPORTANT - This error producing test has to be run very last in order for the other test above to complete successfully.
# This will cause an error b/c we're deriving a changeset between two files with overlapping element IDs while using an OsmMapOperation and the
# elements with the overlapping IDs are not identical. Using an OsmMapOperation requires all map inputs to be read into memory at once, and 
# since we have to preserve element IDs for changeset derivation by keeping the original IDs, the overlapping IDs cannot be loaded.
echo "Deriving changeset with unstreamable op and overlapping element IDs with non-identical elements..."
hoot changeset-derive -C Testing.conf -D convert.ops="hoot::MapCropper;hoot::SetTagValueVisitor" -D crop.bounds="104.8948,38.8519,104.9003,38.85545" -D set.tag.value.visitor.keys=test_key -D set.tag.value.visitor.values=test_val $INPUT_DIR/map1.osm $INPUT_DIR/map3.osm $OUTPUT_DIR/changeset-11.osc
