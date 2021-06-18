#!/bin/bash
set -e

INPUT_DIR=test-files
# This isn't a great recursive input for a test reading unique tag values, since the files are 
# duplicated under the dir.
RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Testing with no keys specified..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm

echo ""
echo "Testing with a single key specified..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --tagKeys highway

echo ""
echo "Testing with a single key specified reading recursively from a directory..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --tagKeys highway --recursive "*"

echo ""
echo "Testing with a single key specified reading recursively from a directory filtered to json files..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --tagKeys highway --recursive "*.json"

echo ""
echo "Testing with multiple keys and inputs specified..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm $INPUT_DIR/DcTigerRoads.osm --tagKeys "highway;name"

echo ""
echo "Testing with a single key and a criterion specified..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --tagKeys highway --criteria HighwayCriterion

echo ""
echo "Testing with a single key and multiple criteria specified..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --tagKeys highway --criteria "HighwayCriterion;WayCriterion"

echo ""
echo "Testing a crit chain..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG -D element.criteria.chain=true $INPUT_DIR/DcGisRoads.osm \
  --tagKeys highway --criteria "HighwayCriterion;PoiCriterion"

echo ""
echo "Testing a non-streamable crit chain that will fail without an input map..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG -D element.criteria.chain=false $INPUT_DIR/DcGisRoads.osm --tagKeys highway \
  --criteria "HighwayCriterion;HighwayWayNodeCriterion"

echo ""
echo "Testing a negated crit..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG -D element.criteria.negate=true $INPUT_DIR/DcGisRoads.osm --tagKeys highway \
  --criteria PoiCriterion

echo ""
echo "Testing key not present..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --tagKeys blah

echo ""
echo "Testing criterion for elements not present..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --tagKeys highway --criteria PoiCriterion

echo ""
echo "Testing with a single key and counting matching elements only..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --tagKeys highway --percentage-of-matching

echo ""
echo "Testing names..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --names

echo ""
echo "Testing types..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --types

echo ""
echo "Testing sorting by value..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --tagKeys highway --sort-by-value

echo ""
echo "Testing limiting..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --names --limit 5

echo ""
echo "Testing tokenization..."
echo ""
hoot tag-distribution $LOG_LEVEL $CONFIG $INPUT_DIR/DcGisRoads.osm --names --tokenize --limit 5
