#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ConvertCmdTest
OUTPUT_DIR=test-output/cmd/slow/ConvertCmdTest2
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

TRANSLATION="-D schema.translation.script=translations"
TRANSLATION_GGDM="$TRANSLATION/GGDMv30.js"
TRANSLATION_TDR4="$TRANSLATION/MGCP_TRD4.js"

# TODO: This has proven unstable some of the time both locally and on Jenkins...not sure why yet.
# Just ensure that no errors show up in the command output.
echo ""
echo "OSM to GPKG..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_GGDM \
  test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA.gpkg
# Do it again to make sure we can overwrite an existing layer.
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_GGDM \
  test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA.gpkg
#hoot diff $LOG_LEVEL $CONFIG -D map.comparator.ignore.tag.keys="UFI" \
#  $INPUT_DIR/ToyTestA.gpkg $OUTPUT_DIR/ToyTestA.gpkg

echo ""
echo "POI Criterion..."
echo ""
# test streaming filter output to pois only
hoot convert $LOG_LEVEL $CONFIG -D writer.xml.sort.by.id="false" -D convert.ops="PoiCriterion" test-files/conflate/unified/AllDataTypesA.osm.pbf $OUTPUT_DIR/AllDataTypesA.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/AllDataTypesAPois.osm $OUTPUT_DIR/AllDataTypesA.osm

echo ""
echo "Check for invalid characters. You should see 3 sets of warnings."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/io/InvalidCharacters.osm $OUTPUT_DIR/InvalidCharacters.osm

echo ""
echo "Translation check..."
echo ""
# This wasn't working at one point.
hoot convert $LOG_LEVEL $CONFIG -D convert.ops=SchemaTranslationVisitor -D schema.translation.direction=toogr $TRANSLATION_TDR4 test-files/ToyTestA.osm $OUTPUT_DIR/translation-check-out.gdb

echo ""
echo "Multi-layer GDB to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/cmd/slow/CountCmdTest/input.gdb $OUTPUT_DIR/multi-layer-gdb-out.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/multi-layer-gdb-out.osm $OUTPUT_DIR/multi-layer-gdb-out.osm

echo ""
echo "Multi-layer GPKG to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/BostonSubsetRoadBuilding_FromOsm.gpkg $OUTPUT_DIR/multi-layer-gpkg-out.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/multi-layer-gpkg-out.osm $OUTPUT_DIR/multi-layer-gpkg-out.osm

echo ""
echo "Multi-layer GDB to OSM single layer only..."
echo ""
hoot convert $LOG_LEVEL $CONFIG "test-files/cmd/slow/CountCmdTest/input.gdb;LAP030" \
  $OUTPUT_DIR/multi-layer-gdb-single-layer-out.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/multi-layer-gdb-single-layer-out.osm $OUTPUT_DIR/multi-layer-gdb-single-layer-out.osm
