#!/bin/bash
set -e

RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Display tag information from a single input..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/jakarta_raya_coastline.shp

echo ""
echo "Display tag information from multiple inputs..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/jakarta_raya_coastline.shp test-files/DcTigerRoads.osm

echo ""
echo "Display tag information recursively from a directory..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --recursive "*"

echo ""
echo "Display tag information recursively from a directory filtered to json files..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --recursive "*.osm"

echo ""
echo "Display tag information with limiting part 1..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/jakarta_raya_coastline.shp --tag-values-limit 1

echo ""
echo "Display tag information with limiting part 2..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --tag-values-limit 5

echo ""
echo "Display tag information for keys only..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys-only

echo ""
echo "Display tag information for keys part 1..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys "foot;highway"

echo ""
echo "Display tag information for keys part 2..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys "foot;HIGHWAY"

echo ""
echo "Display tag information for keys with case insenstivity..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys "foot;HIGHWAY" --case-insensitive

echo ""
echo "Display tag information for non-existent key..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys "blah"

echo ""
echo "Display tag information for keys with partial match part 1..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys "high" --partial-key-match

echo ""
echo "Display tag information for keys with partial match part 2..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys "HIGH" --partial-key-match

echo ""
echo "Display tag information for keys with partial match and case insensitivity..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys "HIGH" --partial-key-match --case-insensitive

echo ""
echo "Display tag information for keys as delimited text..."
echo ""
hoot tag-info $LOG_LEVEL $CONFIG test-files/DcTigerRoads.osm --keys-only --delimited-text
