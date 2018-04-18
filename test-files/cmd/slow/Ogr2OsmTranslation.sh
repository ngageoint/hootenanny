#!/bin/bash
set -e

export OUT=test-output/cmd/Ogr2OsmTranslation
rm -rf $OUT
mkdir -p $OUT
hoot convert-osm2shp "name,poi" test-files/OnePoint.osm $OUT/OnePoint.shp
hoot convert-ogr2osm --warn translations/Identity.js $OUT/OnePointTranslated.osm $OUT/OnePointPoints.shp
hoot map-diff test-output/cmd/Ogr2OsmTranslation/OnePointTranslated.osm test-files/OnePoint.osm

# Make sure we can read all layers from a shapefile
rm -rf test-output/AllDataTypesA.shp test-output/AllDataTypesA.zip
hoot convert test-files/conflate/unified/AllDataTypesA.osm test-output/AllDataTypesA.shp
cd test-output/AllDataTypesA.shp
zip -q ../AllDataTypesA.zip *

cd $HOOT_HOME
hoot convert-ogr2osm --warn translations/Identity.js test-output/AllDataTypesA.osm /vsizip/./test-output/AllDataTypesA.zip
hoot stats test-output/AllDataTypesA.osm | grep -w 'Node Count\|Way Count\|Relation Count'

hoot convert-ogr2osm --warn -D ogr.reader.bounding.box=-104.931,38.823,-104.869,38.877 translations/Identity.js test-output/AllDataTypesABBox.osm /vsizip/./test-output/AllDataTypesA.zip
hoot stats test-output/AllDataTypesABBox.osm | grep -w 'Node Count\|Way Count\|Relation Count'
