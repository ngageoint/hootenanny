#!/bin/bash
set -e

# Test the TDSv61 Translation

inputDir=test-files/TDSv61
outputDir=test-output/tds61_translation

mkdir -p $outputDir
rm -f $outputDir/*

# Normal Hoot options
HOOT_OPT="--info"

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
#HOOT_OPT="--info -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

# Jam all of the shapefiles into one OSM file
hoot ogr2osm $HOOT_OPT $HOOT_HOME/translations/TDSv61.js $outputDir/new_TDSv61.osm $inputDir/*.shp # > tmp/TDSv61toOSM.txt

# Uncomment this to update the OSM file if you edit the shapefiles or the translation file.
#cp $outputDir/new_TDSv61.osm $inputDir/TDSv61.osm

# compareFiles. Test taken from MGCP test
hoot is-match $outputDir/new_TDSv61.osm $inputDir/TDSv61.osm || diff $outputDir/new_TDSv61.osm $inputDir/TDSv61.osm

# Make shapefiles
# NOTE: These are 1 x FCODE / file
hoot osm2ogr $HOOT_OPT -D ogr.thematic.structure=false $HOOT_HOME/translations/TDSv61.js $outputDir/new_TDSv61.osm $outputDir".shp" # > tmp/TDSv61toTDS.txt

##################
# More testing required to get this section of tests to work. In particular, createing two features from one is a problem

# Jam all of the new shapefiles into one OSM file
# hoot ogr2osm $HOOT_OPT $HOOT_HOME/translations/TDSv61.js $outputDir/second_TDSv61.osm $outputDir/*.shp > tmp/TDSv61to_second_OSM.txt

# compareFiles. Test taken from MGCP test
# echo
# echo "Compare the second with orig"
# hoot is-match $outputDir/second_TDSv61_new.osm $inputDir/TDSv61.osm || diff $outputDir/second_TDSv61.osm $inputDir/TDSv61.osm
# echo
# echo "Compare the second with new"
# hoot is-match $outputDir/second_TDSv61.osm $outputDir/new_TDSv61.osm || diff $outputDir/second_TDSv61.osm $outputDir/new_TDSv61.osm
#
# echo
# echo



# This is commented out until Jenkins has python-gdal support
#
# Now look at the individual shapefiles
# for x in $inputDir/*.shp; do
#    echo $(basename $x) "  Forward"
#    $HOOT_HOME/scripts/util/CompareShapefiles.py  $x $outputDir/$(basename $x)
#    echo $(basename $x) "  Backward"
#    $HOOT_HOME/scripts/util/CompareShapefiles.py  $outputDir/$(basename $x) $x
#    echo
# done

