#!/bin/bash
set -e

# Test the TDSv40 Translation

inputDir=test-files/cmd/slow/TDSv40
outputDir=test-output/cmd/slow/tdsv40_translation

TRANS=$HOOT_HOME/translations/TDSv40.js

mkdir -p $outputDir
rm -f $outputDir/*

# Normal Hoot options
HOOT_OPT="--warn  -C Testing.conf"

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
#HOOT_OPT="--info -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

# Script to compare shapefiles. NOTE: This might not run on Jenkins.
COMPARE_SHAPE=$HOOT_HOME/scripts/util/CompareShapefiles.py

##### Start Tests #####
#  jam all of the shapefiles into one OSM file
hoot convert $HOOT_OPT -D schema.translation.script=$TRANS $inputDir/*.shp $outputDir/new_TDSv40.osm # > tmp/TDSv40_to_OSM.txt

# When the translation changes, uncomment this to update the OSM file
#cp $outputDir/new_TDSv40.osm $inputDir/TDSv40X.osm

# compareFiles. Test taken from MGCP test
hoot diff -C Testing.conf $outputDir/new_TDSv40.osm $inputDir/TDSv40.osm || diff $outputDir/new_TDSv40.osm $inputDir/TDSv40.osm

# Make shapefiles
# NOTE: These are thematic.
hoot convert --debug -C Testing.conf -D schema.translation.script=$TRANS $outputDir/new_TDSv40.osm $outputDir".shp" # > tmp/TDSv40_toTDSv40.txt

##################
# More testing required to get this section of tests to work. In particular, createing two features from one is a problem

# Jam all of the new shapefiles into one OSM file
# hoot convert-ogr2osm $HOOT_OPT $TRANS $outputDir/second_TDSv40.osm $outputDir/*.shp > tmp/TDSv40_to_second_OSM.txt

# echo
# echo "Compare the second with orig"
# hoot diff $outputDir/second_TDSv40_new.osm $inputDir/TDSv40.osm || diff $outputDir/second_TDSv40.osm $inputDir/TDSv40.osm
# echo
# echo "Compare the second with new"
# hoot diff $outputDir/second_TDSv40.osm $outputDir/new_TDSv40.osm || diff $outputDir/second_TDSv40.osm $outputDir/new_TDSv40.osm
# echo
# echo


# This is commented out until Jenkins has python-gdal support
#
# Now look at the individual shapefiles
#for x in $inputDir/*.shp; do
#    echo $(basename $x) "  Forward"
#    $COMPARE_SHAPE  $x $outputDir/$(basename $x)
#    echo $(basename $x) "  Backward"
#    $COMPARE_SHAPE  $outputDir/$(basename $x) $x
#    echo
#done
