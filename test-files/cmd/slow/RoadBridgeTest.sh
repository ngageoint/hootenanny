#!/bin/bash
set -e

# Test translating one feature into two features.

INPUT=test-files/RoadBridge.osm
OUTPUT=test-output/cmd/slow/RoadBridge

rm -rf $OUTPUT
mkdir -p test-output/cmd/slow

# Make shapefiles
hoot convert-osm2ogr -D ogr.thematic.structure=false $HOOT_HOME/translations/HootTest.js $INPUT $OUTPUT".shp"

hoot stats --brief $OUTPUT/BRIDGE_C.shp | grep Count
hoot stats --brief $OUTPUT/CART_TRACK_C.shp | grep Count
hoot stats --brief $OUTPUT/RAILWAY_C.shp | grep Count
hoot stats --brief $OUTPUT/ROAD_C.shp | grep Count

