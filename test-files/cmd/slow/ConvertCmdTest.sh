#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/ConvertCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

CONFIG="-C Testing.conf"

echo "Multiple OSM to single OSM..."
hoot convert $CONFIG test-files/ToyTestA.osm test-files/ToyTestB.osm $OUTPUT_DIR/ToyTestC.osm
hoot diff $CONFIG test-files/cmd/slow/ConvertCmdTest/ToyTestC.osm $OUTPUT_DIR/ToyTestC.osm

echo "PBF to OSM..."
hoot convert $CONFIG test-files/ToyTestA.osm.pbf $OUTPUT_DIR/ToyTestA.osm
cat $OUTPUT_DIR/ToyTestA.osm

echo "PBF to SHP..."
hoot convert $CONFIG test-files/ToyTestA.osm.pbf $OUTPUT_DIR/ToyTestA.shp

echo "SHP to OSM..."
# comparing shapefiles is kinda hard. We'll convert it to a .osm file first.
hoot convert $CONFIG $OUTPUT_DIR/ToyTestA.shp $OUTPUT_DIR/ToyTestAShp.osm
hoot diff $CONFIG $OUTPUT_DIR/ToyTestAShp.osm $OUTPUT_DIR/ToyTestAShp.osm

echo "Streaming XML read and write..."
hoot convert $CONFIG -D writer.xml.sort.by.id="false" test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm
hoot diff $CONFIG test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm

echo "POI Criterion..."
# test streaming filter output to pois only
hoot convert $CONFIG -D writer.xml.sort.by.id="false" -D convert.ops="hoot::PoiCriterion" test-files/conflate/unified/AllDataTypesA.osm.pbf $OUTPUT_DIR/AllDataTypesA.osm
hoot diff $CONFIG test-files/cmd/slow/ConvertCmdTest/AllDataTypesAPois.osm $OUTPUT_DIR/AllDataTypesA.osm

echo "Check for invalid characters. You should see 3 sets of warnings."
hoot convert $CONFIG test-files/io/InvalidCharacters.osm $OUTPUT_DIR/InvalidCharacters.osm

echo "Translation check..."
# This wasn't working at one point.
hoot convert $CONFIG -D convert.ops=hoot::SchemaTranslationVisitor -D schema.translation.direction=toogr -D schema.translation.script=translations/MGCP_TRD4.js test-files/ToyTestA.osm $OUTPUT_DIR/translation-check-out.gdb
