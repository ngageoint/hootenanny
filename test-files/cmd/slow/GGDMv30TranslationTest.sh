#!/bin/bash
set -e

# Test the GGDMv30 Translation

inputDir=test-files/GGDMv30
outputDir=test-output/ggdm30_translation

# Script to compare shapefiles. NOTE: This might not run on Jenkins.
COMPARE_SHAPE=$HOOT_HOME/scripts/util/CompareShapefiles.py

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
#HOOT_OPT="--info -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

# Normal Hoot options
HOOT_OPT="--info"

mkdir -p $outputDir
rm -f $outputDir/*

##### Start of initial setup #####
# Un-comment and run these commands when the source OSM file changes.

# Make shapefiles from the OSM file and then update the input shapefiles
# hoot osm2ogr $HOOT_OPT $HOOT_HOME/translations/GGDMv30.js $inputDir/GGDMv30.osm $outputDir".shp"
# cp $outputDir/* $inputDir
# rm -f $outputDir/*
##### End of initial setup #####

#  Jam all of the shapefiles into one OSM file
hoot ogr2osm $HOOT_OPT $HOOT_HOME/translations/GGDMv30.js $outputDir/new_GGDMv30.osm $inputDir/*.shp

# Uncomment this to update the OSM file if you edit the shapefiles.
#cp $outputDir/new_GGDMv30.osm $inputDir/GGDMv30.osm

# Compare the new and old OSM files
hoot is-match $outputDir/new_GGDMv30.osm $inputDir/GGDMv30.osm || diff $outputDir/new_GGDMv30.osm $inputDir/GGDMv30.osm

# Make shapefiles from the new OSM file
# NOTE: This assumes that outputDir does not have any shapefiles in it!
hoot osm2ogr $HOOT_OPT $HOOT_HOME/translations/GGDMv30.js $outputDir/new_GGDMv30.osm $outputDir".shp"

#
# This is commented out until Jenkins has python-gdal support
#
# Now look at the individual shapefiles
for x in $inputDir/*.shp; do
   echo $(basename $x) "  Forward"
   $COMPARE_SHAPE  $x $outputDir/$(basename $x)
   echo $(basename $x) "  Backward"
   $COMPARE_SHAPE  $outputDir/$(basename $x) $x
   echo
done

