#!/bin/bash
set -e

export OUT=test-output/cmd/slow/Ogr2OsmTranslation
rm -rf $OUT
mkdir -p $OUT
hoot convert -D shape.file.writer.cols="name;poi" test-files/OnePoint.osm $OUT/OnePoint.shp
hoot convert --warn -D schema.translation.script=translations/Identity.js $OUT/OnePointPoints.shp $OUT/OnePointTranslated.osm
hoot diff test-output/cmd/slow/Ogr2OsmTranslation/OnePointTranslated.osm test-files/OnePoint.osm

# Make sure we can read all layers from a shapefile
rm -rf test-output/AllDataTypesA.shp test-output/AllDataTypesA.zip
hoot convert test-files/conflate/unified/AllDataTypesA.osm test-output/AllDataTypesA.shp
cd test-output/AllDataTypesA.shp
zip -q ../AllDataTypesA.zip *

cd $HOOT_HOME
hoot convert --warn -D schema.translation.script=translations/Identity.js /vsizip/./test-output/AllDataTypesA.zip test-output/AllDataTypesA.osm
hoot stats --brief test-output/AllDataTypesA.osm | grep -w 'Nodes\|Ways\|Relations'

hoot convert --warn -D schema.translation.script=translations/Identity.js -D ogr.reader.bounding.box=-104.931,38.823,-104.869,38.877 /vsizip/./test-output/AllDataTypesA.zip test-output/AllDataTypesABBox.osm
hoot stats --brief test-output/AllDataTypesABBox.osm | grep -w 'Nodes\|Ways\|Relations'
