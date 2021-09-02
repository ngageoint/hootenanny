#!/bin/bash
set -e

INPUTDIR=$HOOT_HOME/test-files/cmd/quick
OUTPUTDIR=$HOOT_HOME/test-output/cmd/quick/GeoNames

# Raw files
# NOTE: The Geonames.org file is used in UI tests so it hasn't been moved
GEONAMES_ORG=$INPUTDIR/ConvertGeoNames.geonames
GEONAMES_MIL=$INPUTDIR/ConvertGeoNames/ConvertGeoNamesMil.geonames

# Output files
OUT_ORG=$OUTPUTDIR/GeoNames.osm
OUT_ORG_2=$OUTPUTDIR/GeoNames2.osm
OUT_MIL=$OUTPUTDIR/GeoNamesMil.osm

# Gold copy OSM
GOLD_ORG=$INPUTDIR/ConvertGeoNames/GeoNames.osm
GOLD_MIL=$INPUTDIR/ConvertGeoNames/GeoNamesMil.osm

mkdir -p $OUTPUTDIR

# GeoNames.org
hoot convert --warn -C Testing.conf -D map.factory.writer=OsmXmlWriter \
  -D convert.ops=SchemaTranslationVisitor \
  -D schema.translation.script=$HOOT_HOME/translations/GeoNames_to_OSM.js \
  $GEONAMES_ORG $OUT_ORG
hoot diff -C Testing.conf $GOLD_ORG $OUT_ORG || diff $GOLD_ORG $OUT_ORG

# same as previous but make DataConverter add the translate visitor in
hoot convert --warn -C Testing.conf -D map.factory.writer=OsmXmlWriter \
  -D schema.translation.script=$HOOT_HOME/translations/GeoNames_to_OSM.js \
  $GEONAMES_ORG $OUT_ORG_2
hoot diff -C Testing.conf $GOLD_ORG $OUT_ORG_2 || diff $GOLD_ORG $OUT_ORG_2

# geonames.nga.mil
hoot convert --warn -C Testing.conf -D map.factory.writer=OsmXmlWriter \
  -D convert.ops=SchemaTranslationVisitor \
  -D schema.translation.script=$HOOT_HOME/translations/GeoNames_to_OSM.js \
  $GEONAMES_MIL $OUT_MIL
hoot diff -C Testing.conf $GOLD_MIL $OUT_MIL || diff $GOLD_MIL $OUT_MIL
