#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ConvertCmdTest
OUTPUT_DIR=test-output/cmd/slow/ConvertCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo "Multiple OSM to single OSM..."
hoot convert $CONFIG test-files/ToyTestA.osm test-files/ToyTestB.osm $OUTPUT_DIR/ToyTestC.osm
hoot diff $CONFIG $INPUT_DIR/ToyTestC.osm $OUTPUT_DIR/ToyTestC.osm

echo "OSM dir with no filter to single OSM..."
# The input dir has duplicated files. Set to not load data source IDs to load in the duplicated 
# elements. The output should have duplicated features in it.
hoot convert $CONFIG -D reader.use.data.source.ids=false $RECURSIVE_INPUT $OUTPUT_DIR/recursive-out-1.osm --recursive "*"
hoot diff $CONFIG $INPUT_DIR/recursive-out-1.osm $OUTPUT_DIR/recursive-out-1.osm

echo "OSM dir with filter to single OSM..."
# In this case there's only one json file in the dir, so the output won't have any duplicated
# elements due to the input filtering.
hoot convert $CONFIG -D reader.use.data.source.ids=false $RECURSIVE_INPUT $OUTPUT_DIR/recursive-out-2.osm --recursive "*.json"
hoot diff $CONFIG $INPUT_DIR/recursive-out-2.osm $OUTPUT_DIR/recursive-out-2.osm

echo "PBF to OSM..."
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm.pbf $OUTPUT_DIR/ToyTestA.osm
cat $OUTPUT_DIR/ToyTestA.osm

echo "PBF to SHP..."
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm.pbf $OUTPUT_DIR/ToyTestA.shp

echo "SHP to OSM..."
# comparing shapefiles is kinda hard. We'll convert it to a .osm file first.
hoot convert $LOG_LEVEL $CONFIG $OUTPUT_DIR/ToyTestA.shp $OUTPUT_DIR/ToyTestAShp.osm
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/ToyTestAShp.osm $OUTPUT_DIR/ToyTestAShp.osm

echo "Streaming XML read and write..."
hoot convert $LOG_LEVEL $CONFIG -D writer.xml.sort.by.id="false" test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm
hoot diff $LOG_LEVEL $CONFIG test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm

echo "POI Criterion..."
# test streaming filter output to pois only
hoot convert $LOG_LEVEL $CONFIG -D writer.xml.sort.by.id="false" -D convert.ops="hoot::PoiCriterion" test-files/conflate/unified/AllDataTypesA.osm.pbf $OUTPUT_DIR/AllDataTypesA.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/AllDataTypesAPois.osm $OUTPUT_DIR/AllDataTypesA.osm

echo "Check for invalid characters. You should see 3 sets of warnings."
hoot convert $LOG_LEVEL $CONFIG test-files/io/InvalidCharacters.osm $OUTPUT_DIR/InvalidCharacters.osm

echo "Translation check..."
# This wasn't working at one point.
hoot convert $LOG_LEVEL $CONFIG -D convert.ops=hoot::SchemaTranslationVisitor -D schema.translation.direction=toogr -D schema.translation.script=translations/MGCP_TRD4.js test-files/ToyTestA.osm $OUTPUT_DIR/translation-check-out.gdb

echo "Multi-layer GDB to OSM..."
hoot convert $LOG_LEVEL $CONFIG test-files/cmd/slow/CountCmdTest/input.gdb $OUTPUT_DIR/multi-layer-gdb-out.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/multi-layer-gdb-out.osm $OUTPUT_DIR/multi-layer-gdb-out.osm
