#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ConvertCmdTest
OUTPUT_DIR=test-output/cmd/slow/ConvertCmdTest3
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

GOLD_FILE_CONTAINER_OUTPUT=$INPUT_DIR/combined_shapefile.osm

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

TRANSLATION="-D schema.translation.script=translations"
TRANSLATION_GGDM="$TRANSLATION/GGDMv30.js"
TRANSLATION_MGCP="$TRANSLATION/MgcpTest.js"
TRANSLATION_DNC="$TRANSLATION/DNC.js"

echo ""
echo "Streaming XML read and write..."
echo ""
hoot convert $LOG_LEVEL $CONFIG -D writer.xml.sort.by.id="false" test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm
hoot diff $LOG_LEVEL $CONFIG test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm

echo ""
echo "ZIP of SHP to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_MGCP \
  test-files/MGCPv3.zip $OUTPUT_DIR/zip_convert.osm
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_CONTAINER_OUTPUT $OUTPUT_DIR/zip_convert.osm

echo ""
echo "TAR of SHP to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_MGCP \
  test-files/MGCPv3.tar $OUTPUT_DIR/tar_convert.osm
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_CONTAINER_OUTPUT $OUTPUT_DIR/tar_convert.osm

echo ""
echo "TAR GZ of SHP to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_MGCP \
  test-files/MGCPv3.tar.gz $OUTPUT_DIR/tar_gz_convert.osm
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_CONTAINER_OUTPUT $OUTPUT_DIR/tar_gz_convert.osm

echo ""
echo "Streaming XML read and GeoJSON write..."
echo ""
hoot convert $CONFIG test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA.geojson
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/ToyTestA.geojson $OUTPUT_DIR/ToyTestA.geojson

echo ""
echo "Osm to FlatGeobuf..."
echo ""
FLATGEOBUF_OPTIONS="-D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D ogr.add.uuid=false"
FLATGEOBUF_OPS="-D convert.ops=DuplicateNodeRemover"
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_GGDM $FLATGEOBUF_OPTIONS test-files/io/GeoJson/AllDataTypes.osm $OUTPUT_DIR/AllDataTypes.fgb
hoot convert $LOG_LEVEL  $CONFIG $TRANSLATION_GGDM $FLATGEOBUF_OPTIONS $OUTPUT_DIR/AllDataTypes.fgb $OUTPUT_DIR/AllDataTypesFgb.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/AllDataTypesFgb.osm $OUTPUT_DIR/AllDataTypesFgb.osm
