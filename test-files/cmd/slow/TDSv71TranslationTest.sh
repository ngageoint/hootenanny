#!/bin/bash
set -e

# Test the TDSv71 Translation

inputDir=$HOOT_HOME/test-files/cmd/slow/TDSv71
outputDir=$HOOT_HOME/test-output/cmd/slow/tds71_translation

TRANS=$HOOT_HOME/translations/TDSv71.js

mkdir -p $outputDir
rm -rf $outputDir/*

# Normal Hoot options
HOOT_OPT="--info -C Testing.conf"

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
# HOOT_OPT+=" -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

COMPARE_OUTPUT=$HOOT_HOME/scripts/schema/compareTrans.py

# Make shapefiles from the OSM file
hoot convert --error -D ogr.compare.output=true  -D ogr.debug.dumptags=true  \
  -D schema.translation.script=$TRANS \
  $inputDir/TDSv71.osm $outputDir/new_TDSv71.shp > $outputDir/x_testfile.txt

# Convert the Shapefiles back to OSM
hoot convert --error -D ogr.compare.output=true  -D ogr.debug.dumptags=true  \
  -D schema.translation.script=$TRANS \
  $outputDir/new_TDSv71/*.shp  $outputDir/new_TDSv71.osm > $outputDir/in_testfile.txt

# Compare what went out to what comes back in_testfile

$COMPARE_OUTPUT $outputDir/x_testfile.txt $outputDir/in_testfile.txt

