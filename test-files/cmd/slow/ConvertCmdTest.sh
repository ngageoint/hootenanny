#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ConvertCmdTest
OUTPUT_DIR=test-output/cmd/slow/ConvertCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

SEPARATE_OUTPUT_INPUT_1=$OUTPUT_DIR/ToyTestA.shp
SEPARATE_OUTPUT_INPUT_2=$OUTPUT_DIR/ToyTestB.osm.pbf
SEPARATE_OUTPUT_OUTPUT_1=$OUTPUT_DIR/ToyTestA-converted.osm
SEPARATE_OUTPUT_OUTPUT_2=$OUTPUT_DIR/ToyTestB-converted.osm
GOLD_FILE_SEPARATE_OUTPUT_1=$OUTPUT_DIR/ToyTestAShp.osm
GOLD_FILE_SEPARATE_OUTPUT_2=test-files/ToyTestB.osm.pbf

RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Multiple OSM to single OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm test-files/ToyTestB.osm $OUTPUT_DIR/ToyTestC.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/ToyTestC.osm $OUTPUT_DIR/ToyTestC.osm

echo ""
echo "Dir with no filter to single OSM..."
echo ""
# The input dir has duplicated files. Set to not load data source IDs to load in the duplicated 
# elements. The output should have duplicated features in it.
hoot convert $LOG_LEVEL $CONFIG -D non.osm.convert.merge.nearby.nodes=false -D reader.use.data.source.ids=false $RECURSIVE_INPUT $OUTPUT_DIR/recursive-out-1.osm --recursive "*"
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/recursive-out-1.osm $OUTPUT_DIR/recursive-out-1.osm

echo ""
echo "Dir with filter to single OSM..."
echo ""
# In this case there's only one json file in the dir, so the output won't have any duplicated
# elements due to the input filtering.
hoot convert $LOG_LEVEL $CONFIG -D reader.use.data.source.ids=false $RECURSIVE_INPUT $OUTPUT_DIR/recursive-out-2.osm --recursive "*.json"
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/recursive-out-2.osm $OUTPUT_DIR/recursive-out-2.osm

echo ""
echo "PBF to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm.pbf $OUTPUT_DIR/ToyTestA.osm
cat $OUTPUT_DIR/ToyTestA.osm

echo ""
echo "PBF to SHP..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm.pbf $SEPARATE_OUTPUT_INPUT_1

echo ""
echo "SHP to OSM..."
echo ""
# comparing shapefiles is kinda hard. We'll convert it to a .osm file first.
hoot convert $LOG_LEVEL $CONFIG $SEPARATE_OUTPUT_INPUT_1 $OUTPUT_DIR/ToyTestAShp.osm
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/ToyTestAShp.osm $OUTPUT_DIR/ToyTestAShp.osm

echo ""
echo "Multiple formats to OSM and writing to separate outputs..."
echo ""
# Copy the inputs to the output directory and work from there, so we don't write to the input dir
# under source control. Note the file comparison here depends on the output of a previous command in
# this test and that input doesn't need to be copied to the output dir.
cp $GOLD_FILE_SEPARATE_OUTPUT_2 $OUTPUT_DIR
hoot convert $LOG_LEVEL $CONFIG $SEPARATE_OUTPUT_INPUT_1 $SEPARATE_OUTPUT_INPUT_2 "osm" --separate-output
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_SEPARATE_OUTPUT_1 $SEPARATE_OUTPUT_OUTPUT_1
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_SEPARATE_OUTPUT_2 $SEPARATE_OUTPUT_OUTPUT_2
