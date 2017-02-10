#!/bin/bash
set -e

# Test translating one feature into two features.

INPUT=test-files/RoadBridge.osm
OUTPUT=test-output/RoadBridge

rm -rf $OUTPUT

# Make shapefiles
hoot --osm2ogr -D ogr.tds.structure=false $HOOT_HOME/translations/HootTest.js $INPUT $OUTPUT".shp"

hoot stats --brief test-output/RoadBridge/BRIDGE_C.shp | grep Count
hoot stats --brief test-output/RoadBridge/CART_TRACK_C.shp | grep Count
hoot stats --brief test-output/RoadBridge/RAILWAY_C.shp | grep Count
hoot stats --brief test-output/RoadBridge/ROAD_C.shp | grep Count

