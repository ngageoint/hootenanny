#!/bin/bash
set -e

export OUT=test-output/cmd/Ogr2OsmTranslation
rm -rf $OUT
mkdir -p $OUT
hoot --osm2shp "name,poi" test-files/OnePoint.osm $OUT/OnePoint.shp
hoot --ogr2osm translations/Identity.js $OUT/OnePointTranslated.osm $OUT/OnePointPoints.shp
hoot --is-match test-output/cmd/Ogr2OsmTranslation/OnePointTranslated.osm test-files/OnePoint.osm

# Make sure we can read all layers from a shapefile
rm -rf test-output/AllDataTypes.shp test-output/AllDataTypesA.zip
hoot --convert test-files/conflate/unified/AllDataTypesA.osm test-output/AllDataTypes.shp
cd test-output/
zip -q AllDataTypesA.zip AllDataTypes.shp/*
cd $HOOT_HOME
hoot --ogr2osm translations/Identity.js test-output/AllDataTypesA.osm /vsizip/./test-output/AllDataTypesA.zip/AllDataTypes.shp
hoot --stats test-output/AllDataTypesA.osm | grep -w 'Node Count\|Way Count\|Relation Count'

hoot --ogr2osm -D ogr.reader.bounding.box=-104.931,38.823,-104.869,38.877 translations/Identity.js test-output/AllDataTypesABBox.osm /vsizip/./test-output/AllDataTypesA.zip/AllDataTypes.shp
hoot --stats test-output/AllDataTypesABBox.osm | grep -w 'Node Count\|Way Count\|Relation Count'
