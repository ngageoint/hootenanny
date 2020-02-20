#!/bin/bash
set -e

INPUT_DIR=test-files

CONFIG="--warn -C Testing.conf"

echo ""
echo "#1:"
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway

echo ""
echo "#2:"
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway --sort-by-value

echo ""
echo "#3:"
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway hoot::PoiCriterion

echo ""
echo "#4:"
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm --names

echo ""
echo "#5:"
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm --names --limit 5

echo ""
echo "#6:"
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm --names --tokenize --limit 5

echo ""
echo "#7:"
hoot tag-distribution $CONFIG "$INPUT_DIR/DcGisRoads.osm;$INPUT_DIR/DcTigerRoads.osm" "highway;name"

echo ""
echo "#8:"
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm blah
