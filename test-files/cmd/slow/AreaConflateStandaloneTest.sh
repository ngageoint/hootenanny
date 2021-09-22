#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR_1=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest
IN_DIR_2=test-files/cmd/slow/AreaConflateStandaloneTest
OUT_DIR=test-output/cmd/slow/AreaConflateStandaloneTest
mkdir -p $OUT_DIR

TO_VALIDATE_1=test-output/cmd/slow/AreaConflateStandaloneTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/AreaConflateStandaloneTest/output-validation-report
hoot conflate --warn -C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf \
  -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,Area.js" \
  -D merger.creators="ScriptMergerCreator" $IN_DIR_1/PoiPolygon1.osm $IN_DIR_1/PoiPolygon2.osm \
  $OUT_DIR/output.osm
hoot diff -C Testing.conf $OUT_DIR/output.osm $IN_DIR_2/output.osm || diff $OUT_DIR/output.osm $IN_DIR_2/output.osm
