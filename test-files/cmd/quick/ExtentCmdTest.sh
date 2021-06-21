#!/bin/bash
set -e

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

RECURSIVE_INPUT=test-files/cmd/quick/ExtentCmdTest

echo ""
echo "Calculating the extent of one input..."
echo ""
hoot extent $LOG_LEVEL $CONFIG test-files/DcGisRoads.osm

echo ""
echo "Calculating the extent of multiple inputs..."
echo ""
hoot extent $LOG_LEVEL $CONFIG test-files/DcGisRoads.osm test-files/ToyTestA.osm
<<<<<<< HEAD

echo ""
echo "Calculating the extent of data recursively in a directory structure..."
echo ""
hoot extent $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --recursive "*"

echo ""
echo "Calculating the extent of data recursively in a directory structure with one filter..."
echo ""
# Since the only files in the input dir are *.osm, this has the same output as the previously 
# executed command.
hoot extent $LOG_LEVEL $CONFIG $RECURSIVE_INPUT --recursive "*.osm"
