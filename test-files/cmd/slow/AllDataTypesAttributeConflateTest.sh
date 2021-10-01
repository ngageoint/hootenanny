#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/AllDataTypesAttributeConflateTest
OUT_DIR=test-output/cmd/slow/AllDataTypesAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"
source scripts/core/ScriptTestUtils.sh

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf \
  -D writer.include.debug.tags=true \
  -D match.creators="HighwayMatchCreator;BuildingMatchCreator;ScriptMatchCreator,Poi.js" \
  -D merger.creators="HighwayMergerCreator;BuildingMergerCreator;ScriptMergerCreator" \
  test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm

validateTestOutput $OUT_DIR/output.osm $OUT_DIR/output-validation-report \
  $OUT_DIR/output-validated.osm $IN_DIR/output-validation-report
