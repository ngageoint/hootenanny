#!/bin/bash
set -e

INPUT_DIR=test-files

CONFIG="--warn -C Testing.conf"

echo ""
echo "Testing with no keys specified..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm ""

echo ""
echo "Testing with a single key specified..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway

echo ""
echo "Testing with multiple keys and inputs specified..."
echo ""
hoot tag-distribution $CONFIG "$INPUT_DIR/DcGisRoads.osm;$INPUT_DIR/DcTigerRoads.osm" "highway;name"

echo ""
echo "Testing with a single key and a criterion specified..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway HighwayCriterion

echo ""
echo "Testing with a single key and multiple criteria specified..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway "HighwayCriterion;WayCriterion"

echo ""
echo "Testing a crit chain..."
echo ""
hoot tag-distribution $CONFIG -D element.criteria.chain=true $INPUT_DIR/DcGisRoads.osm highway "HighwayCriterion;PoiCriterion"

echo ""
echo "Testing a negated crit..."
echo ""
hoot tag-distribution $CONFIG -D element.criteria.negate=true $INPUT_DIR/DcGisRoads.osm highway PoiCriterion

echo ""
echo "Testing key not present..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm blah

echo ""
echo "Testing criterion for elements not present..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway PoiCriterion

echo ""
echo "Testing with a single key and counting matching elements only..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway --percentage-of-matching

echo ""
echo "Testing names..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm --names

echo ""
echo "Testing types..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm --types

echo ""
echo "Testing sorting by value..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm highway --sort-by-value

echo ""
echo "Testing limiting..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm --names --limit 5

echo ""
echo "Testing tokenization..."
echo ""
hoot tag-distribution $CONFIG $INPUT_DIR/DcGisRoads.osm --names --tokenize --limit 5
