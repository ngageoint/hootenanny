#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/AreaAttributeConflateStandaloneTest
OUT_DIR=test-output/cmd/slow/AreaAttributeConflateStandaloneTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,Area.js" -D merger.creators="ScriptMergerCreator" test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm

if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $CONFIG $OUT_DIR/output.osm \
    --report-output $OUT_DIR/output-validation-report --output $OUT_DIR/output-validated.osm
  diff $IN_DIR/output-validation-report $OUT_DIR/output-validation-report
fi
