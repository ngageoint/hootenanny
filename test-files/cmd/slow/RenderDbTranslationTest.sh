#!/bin/bash
set -e

IN=test-files/renderDb.osm
OUT=test-output/cmd/slow/translation/RenderDb

function compareFiles {
    hoot diff -C Testing.conf $1 $2 || diff $1 $2
}

CONFIG="-C NodeExport.conf -C Testing.conf"

# LOTS of debug output
#CONFIG="-D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

mkdir -p $(dirname ${OUT})
rm -rf "${OUT}_in.osm" "${OUT}_out.osm"


hoot convert \
  $CONFIG \
  -D schema.translation.direction=toogr \
  -D convert.ops=hoot::SchemaTranslationVisitor \
  -D schema.translation.script=$HOOT_HOME/translations/RenderDb.js \
  ${IN} \
  "${OUT}_out.osm"

hoot convert \
  $CONFIG \
  -D schema.translation.direction=toosm \
  -D convert.ops=hoot::SchemaTranslationVisitor \
  -D schema.translation.script=$HOOT_HOME/translations/RenderDb.js \
  "${OUT}_out.osm" \
  "${OUT}_in.osm"

#echo $IN  $OUT
compareFiles "${OUT}_in.osm" ${IN}
