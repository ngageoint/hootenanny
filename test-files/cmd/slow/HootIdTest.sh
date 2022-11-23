#!/bin/bash
set -e

# Test adding a hood:id tag to output
INPUT=test-files/building_Valqueire.osm
OUTPUT=test-output/cmd/slow/hootId

TRANS_GGDM="-D schema.translation.script=$HOOT_HOME/translations/GGDMv30.js"

rm -rf $OUTPUT
mkdir -p $OUTPUT

CONFIG="-C Testing.conf"

# Make translated shapefiles
hoot convert $CONFIG $TRANS_GGDM \
  -D writer.include.id.tag=true \
  -D ogr.thematic.structure=false \
  $INPUT $OUTPUT/ogr.shp

# Make translated GeoJSON
hoot convert $CONFIG $TRANS_GGDM \
  -D schema.translation.direction=toogr \
  -D convert.ops='AddElementIdVisitor;SchemaTranslationVisitor' \
  -D ogr.thematic.structure=false \
  $INPUT $OUTPUT/out.geojson

# OSM to translated OSM
hoot convert $CONFIG $TRANS_GGDM \
  -D schema.translation.direction=toogr \
  -D convert.ops='AddElementIdVisitor;SchemaTranslationVisitor' \
  -D ogr.thematic.structure=false \
  $INPUT $OUTPUT/out_trans.osm

# OSM to OSM - visit
hoot convert $CONFIG \
  -D convert.ops='AddElementIdVisitor' \
  $INPUT $OUTPUT/out_visit.osm

# OSM to OSM - config option
hoot convert $CONFIG \
  -D writer.include.id.tag=true \
  $INPUT $OUTPUT/out_config.osm


# Now check that the inputs have hoot:id tags
ogrinfo -ro -al -geom=no $OUTPUT/ogr/BUILDING_P.shp  > $OUTPUT/out_shp.txt

echo "Expecting five outputs to have hoot:id tags."

if [ `grep -l hoot:id $OUTPUT/out* | wc -l` -eq "5" ]; then
  echo "Found it in all five outputs"
else
  echo "Nope"
  exit 1;
fi
