#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR_1=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest
IN_DIR_2=test-files/cmd/slow/AreaConflateStandaloneTest
OUT_DIR=test-output/cmd/slow/AreaConflateStandaloneTest
mkdir -p $OUT_DIR

LOG_LEVEL="--warn"
CONFIG="-C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf \
  -D uuid.helper.repeatable=true -D match.creators=ScriptMatchCreator,Area.js \
  -D merger.creators=ScriptMergerCreator"

hoot conflate $LOG_LEVEL $CONFIG $IN_DIR_1/PoiPolygon1.osm $IN_DIR_1/PoiPolygon2.osm $OUT_DIR/output.osm
hoot diff -C Testing.conf $OUT_DIR/output.osm $IN_DIR_2/output.osm || diff $OUT_DIR/output.osm $IN_DIR_2/output.osm

if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUT_DIR/output.osm \
    --report-output $OUT_DIR/output-validation-report --output $OUT_DIR/output-validated.osm
  diff $IN_DIR_2/output-validation-report $OUT_DIR/output-validation-report
fi
