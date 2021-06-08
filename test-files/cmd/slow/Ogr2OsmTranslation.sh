#!/bin/bash
set -e

export OUT=test-output/cmd/slow/Ogr2OsmTranslation
rm -rf $OUT
mkdir -p $OUT
HOOT_OPTS="-C Testing.conf -D duplicate.node.remover.distance.threshold=1.0"

echo "Converting osm to shp with cols..."
hoot convert --warn $HOOT_OPTS -D shape.file.writer.cols="name;poi" test-files/OnePoint.osm $OUT/OnePoint.shp
hoot convert --warn $HOOT_OPTS -D schema.translation.script=translations/Identity.js $OUT/OnePointPoints.shp $OUT/OnePointTranslated.osm
hoot diff $HOOT_OPTS test-output/cmd/slow/Ogr2OsmTranslation/OnePointTranslated.osm test-files/OnePoint.osm

# Make sure we can read all layers from a shapefile
echo "Converting osm to shp..."
rm -rf test-output/AllDataTypesA.shp test-output/AllDataTypesA.zip
hoot convert --warn $HOOT_OPTS test-files/conflate/unified/AllDataTypesA.osm test-output/AllDataTypesA.shp
cd test-output/AllDataTypesA.shp
zip -q ../AllDataTypesA.zip *

cd $HOOT_HOME

echo "Converting zip to osm..."
hoot convert --warn $HOOT_OPTS -D schema.translation.script=translations/Identity.js /vsizip/./test-output/AllDataTypesA.zip test-output/AllDataTypesA.osm
hoot stats $HOOT_OPTS --brief test-output/AllDataTypesA.osm | grep -w 'Nodes\|Ways\|Relations'

echo "Converting zip to osm with bounds..."
hoot convert --warn $HOOT_OPTS -D schema.translation.script=translations/Identity.js -D bounds=-104.931,38.823,-104.869,38.877 /vsizip/./test-output/AllDataTypesA.zip test-output/AllDataTypesABBox.osm
hoot stats $HOOT_OPTS --brief test-output/AllDataTypesABBox.osm | grep -w 'Nodes\|Ways\|Relations'
