#!/bin/bash
set -e

# Test TDSv61 Translation

inputDir=test-files/NFDD_TDSv61
outputDir=test-output/tds61_translation

mkdir -p $outputDir
rm -f $outputDir/*

#  jam all of the shapefiles into one OSM file
hoot --ogr2osm $HOOT_HOME/translations/TDSv61.js $outputDir/tds61.osm $inputDir/*.shp

# compareFiles. Test taken from MGCP test
hoot --is-match $outputDir/tds61.osm $inputDir/NFDDv61.osm || diff $outputDir/tds61.osm $inputDir/NFDDv61.osm

# Make shapefiles
hoot --osm2ogr $HOOT_HOME/translations/TDSv61.js $outputDir/tds61.osm $outputDir".shp"

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

