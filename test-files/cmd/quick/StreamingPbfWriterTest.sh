#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/quick/StreamingPbfWriterTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE=test-files/cmd/quick/ConvertGeoNames.geonames
 
#had trouble directly comparing geonames to pbf, so converting both to osm for comparison
hoot convert $INPUT_FILE $OUTPUT_DIR/out.osm.pbf #leave this one at info, so you get the "streaming" message
hoot convert --warn $OUTPUT_DIR/out.osm.pbf $OUTPUT_DIR/out-1.osm
hoot convert --warn $INPUT_FILE $OUTPUT_DIR/out-2.osm
hoot map-diff --warn $OUTPUT_DIR/out-1.osm $OUTPUT_DIR/out-2.osm
