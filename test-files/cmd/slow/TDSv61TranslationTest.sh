#!/bin/bash
set -e

# Test the TDSv61 Translation

inputDir=test-files/TDSv61
outputDir=test-output/tds61_translation

TRANS=$HOOT_HOME/translations/TDSv61.js

mkdir -p $outputDir
rm -rf $outputDir/*

# Normal Hoot options
HOOT_OPT="--warn"

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
#HOOT_OPT="--info -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

# Script to compare shapefiles. NOTE: This might not run on Jenkins.
COMPARE_SHAPE=$HOOT_HOME/scripts/util/CompareShapefiles.py

##### Start Tests #####
# Jam all of the shapefiles into one OSM file
hoot convert-ogr2osm $HOOT_OPT $TRANS $outputDir/new_TDSv61.osm $inputDir/*.shp # > tmp/TDSv61_to_OSM.txt

# Uncomment this to update the OSM file if you edit the shapefiles or the translation file.
#cp $outputDir/new_TDSv61.osm $inputDir/TDSv61.osm

# compareFiles. Test taken from MGCP test
hoot map-diff $outputDir/new_TDSv61.osm $inputDir/TDSv61.osm || diff $outputDir/new_TDSv61.osm $inputDir/TDSv61.osm

# Make shapefiles
# NOTE: These are 1 x FCODE / file and it assumes that the output dir doesn't have any shapefiles in it
hoot convert-osm2ogr --debug -D ogr.thematic.structure=false $TRANS $outputDir/new_TDSv61.osm $outputDir".shp" # > tmp/TDSv61_to_TDS.txt

# Make shapefiles - Thematic
# hoot convert-osm2ogr $HOOT_OPT $TRANS $outputDir/new_TDSv61.osm $outputDir/"new_files.shp" > tmp/TDSv61_to_TDS2.txt
# hoot convert-ogr2osm $HOOT_OPT $TRANS $outputDir/second_TDSv61.osm $outputDir/new_files/*.shp > tmp/TDSv61_second_to_OSM.txt

##################
# More testing required to get this section of tests to work. In particular, createing two features from one is a problem

# Jam all of the new shapefiles into one OSM file
# hoot convert-ogr2osm $HOOT_OPT $TRANS $outputDir/second_TDSv61.osm $outputDir/*.shp > tmp/TDSv61_to_second_OSM.txt

# echo
# echo "Compare the second with orig"
# hoot map-diff $outputDir/second_TDSv61_new.osm $inputDir/TDSv61.osm || diff $outputDir/second_TDSv61.osm $inputDir/TDSv61.osm
# echo
# echo "Compare the second with new"
# hoot map-diff $outputDir/second_TDSv61.osm $outputDir/new_TDSv61.osm || diff $outputDir/second_TDSv61.osm $outputDir/new_TDSv61.osm
# echo
# echo


# This is commented out until Jenkins has python-gdal support
#
# Now look at the individual shapefiles
# for x in $inputDir/*.shp; do
#    echo $(basename $x) "  Forward"
#    $COMPARE_SHAPE  $x $outputDir/$(basename $x)
#    echo $(basename $x) "  Backward"
#    $COMPARE_SHAPE  $outputDir/$(basename $x) $x
#    echo
# done
