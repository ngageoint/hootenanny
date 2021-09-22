#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/PowerLineConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineConflateTest
mkdir -p $OUT_DIR

CONFIG="-C Testing.conf"

hoot conflate --warn $CONFIG -C UnifyingAlgorithm.conf -C ReferenceConflation.conf \
  -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,PowerLine.js" \
  -D merger.creators="ScriptMergerCreator" $IN_DIR/power-line-1.osm $IN_DIR/power-line-2.osm \
  $OUT_DIR/output.osm
hoot diff $CONFIG $OUT_DIR/output.osm $IN_DIR/output.osm || diff $OUT_DIR/output.osm $IN_DIR/output.osm

if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate --warn $CONFIG $OUT_DIR/output.osm \
    --report-output $OUT_DIR/output-validation-report --output $OUT_DIR/output-validated.osm
  diff $IN_DIR/output-validation-report $OUT_DIR/output-validation-report
fi
