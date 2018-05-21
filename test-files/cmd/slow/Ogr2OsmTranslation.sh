#!/bin/bash
set -e

export OUT=test-output/cmd/Ogr2OsmTranslation
rm -rf $OUT
mkdir -p $OUT
hoot convert test-files/OnePoint.osm $OUT/OnePoint.shp --cols "name,poi"
hoot convert --warn $OUT/OnePointPoints.shp $OUT/OnePointTranslated.osm --trans translations/Identity.js
hoot map-diff test-output/cmd/Ogr2OsmTranslation/OnePointTranslated.osm test-files/OnePoint.osm

# Make sure we can read all layers from a shapefile
rm -rf test-output/AllDataTypesA.shp test-output/AllDataTypesA.zip
hoot convert test-files/conflate/unified/AllDataTypesA.osm test-output/AllDataTypesA.shp
cd test-output/AllDataTypesA.shp
zip -q ../AllDataTypesA.zip *

cd $HOOT_HOME
hoot convert --warn /vsizip/./test-output/AllDataTypesA.zip test-output/AllDataTypesA.osm --trans translations/Identity.js 
hoot stats test-output/AllDataTypesA.osm | grep -w 'Node Count\|Way Count\|Relation Count'

hoot convert --warn -D ogr.reader.bounding.box=-104.931,38.823,-104.869,38.877 /vsizip/./test-output/AllDataTypesA.zip test-output/AllDataTypesABBox.osm --trans translations/Identity.js
hoot stats test-output/AllDataTypesABBox.osm | grep -w 'Node Count\|Way Count\|Relation Count'
