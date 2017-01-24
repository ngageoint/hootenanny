#!/bin/bash
set -e

export OUT=test-output/cmd/BigIteratorTest
rm -rf $OUT
mkdir -p $OUT
export SHP=test-files/jakarta_raya_coastline.shp
export SHP_EMPTY=test-files/empty_shapefile.shp

# Test empty file w/ big cat & big ogr2osm
hoot --big-ogr2osm translations/Identity.js $OUT/BigOgr2OsmShpEmpty.osm.pbf $SHP_EMPTY
hoot --stats --brief $OUT/BigOgr2OsmShpEmpty.osm.pbf | grep Count

hoot --big-cat $OUT/BigCatShpEmpty.osm.pbf $OUT/BigOgr2OsmShpEmpty.osm.pbf
hoot --stats --brief $OUT/BigCatShpEmpty.osm.pbf | grep Count

# Test populated file w/ big cat & big ogr2osm
hoot --big-ogr2osm translations/Identity.js $OUT/BigOgr2OsmShp.osm.pbf $SHP
hoot --stats --brief $OUT/BigOgr2OsmShp.osm.pbf | grep Count

hoot --big-cat $OUT/BigCatShp.osm.pbf $OUT/BigOgr2OsmShp.osm.pbf
hoot --stats --brief $OUT/BigCatShp.osm.pbf | grep Count

