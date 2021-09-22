#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/AllDataTypesAttributeConflateTest
OUT_DIR=test-output/cmd/slow/AllDataTypesAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

TO_VALIDATE_1=test-output/cmd/slow/AllDataTypesAttributeConflateTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/AllDataTypesAttributeConflateTest/output-validation-report
hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf \
  -D writer.include.debug.tags=true \
  -D match.creators="HighwayMatchCreator;BuildingMatchCreator;ScriptMatchCreator,Poi.js" \
  -D merger.creators="HighwayMergerCreator;BuildingMergerCreator;ScriptMergerCreator" \
  test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm $OUT_DIR/output.osm

hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
