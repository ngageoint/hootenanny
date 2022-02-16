#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/AreaAttributeConflateStandaloneTest
OUT_DIR=test-output/cmd/slow/AreaAttributeConflateStandaloneTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"
source scripts/core/ScriptTestUtils.sh

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf \
  -D conflate.use.data.source.ids.1="false" -D conflate.use.data.source.ids.2="true" \
  -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,Area.js" -D merger.creators="ScriptMergerCreator" \
  test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
