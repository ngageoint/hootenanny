#!/bin/bash
set -e

# Test NFDD Translation

inputDir=test-files/TDSv40
outputDir=test-output/TDSv40_translation

mkdir -p $outputDir
rm -f $outputDir/*

#  jam all of the shapefiles into one OSM file
hoot --ogr2osm $HOOT_HOME/translations/TDSv40.js $outputDir/TDSv40.osm $inputDir/*.shp

# When the translation changes, uncomment this to update the OSM file
#cp $outputDir/TDSv40.osm $inputDir

# compareFiles. Test taken from MGCP test
hoot --is-match $outputDir/TDSv40.osm $inputDir/TDSv40.osm || diff $outputDir/TDSv40.osm $inputDir/TDSv40.osm

# Make shapefiles
hoot --osm2ogr $HOOT_HOME/translations/TDSv40.js $outputDir/TDSv40.osm $outputDir".shp"

#
# This is commented out until Jenkins has python-gdal support
#
# Now look at the individual shapefiles
#for x in $inputDir/*.shp; do
#    echo $(basename $x) "  Forward"
#    $HOOT_HOME/scripts/util/CompareShapefiles.py  $x $outputDir/$(basename $x)
#    echo $(basename $x) "  Backward"
#    $HOOT_HOME/scripts/util/CompareShapefiles.py  $outputDir/$(basename $x) $x
#    echo
#done

