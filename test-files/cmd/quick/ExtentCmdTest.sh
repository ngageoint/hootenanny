#!/bin/bash
set -e

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Calculating the extent of one input..."
echo ""
hoot extent $LOG_LEVEL $CONFIG test-files/DcGisRoads.osm

echo ""
echo "Calculating the extent of multiple inputs..."
echo ""
hoot extent $LOG_LEVEL $CONFIG test-files/DcGisRoads.osm test-files/ToyTestA.osm
