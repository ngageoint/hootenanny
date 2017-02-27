#!/bin/bash
set -e

# Test NFDD Translation

inputDir=test-files/NFDD_TDSv4
outputDir=test-output/nfdd_translation

mkdir -p $outputDir
rm -f $outputDir/*

#  jam all of the shapefiles into one OSM file
hoot --ogr2osm $HOOT_HOME/translations/TDSv40.js $outputDir/nfdd.osm $inputDir/*.shp

# compareFiles. Test taken from MGCP test
hoot --is-match $outputDir/nfdd.osm $inputDir/nfdd.osm || diff $outputDir/nfdd.osm $inputDir/nfdd.osm

# Make shapefiles
hoot --osm2ogr $HOOT_HOME/translations/TDSv40.js $outputDir/nfdd.osm $outputDir".shp"

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

