#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/PowerLineAttributeConflateTest
IN_DIR_2=test-files/cmd/slow/PowerLineConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf \
  -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,PowerLine.js" \
  -D merger.creators="ScriptMergerCreator" $IN_DIR_2/power-line-1.osm $IN_DIR_2/power-line-2.osm \
  $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm

if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $CONFIG $OUT_DIR/output.osm \
    --report-output $OUT_DIR/output-validation-report --output $OUT_DIR/output-validated.osm
  diff $IN_DIR/output-validation-report $OUT_DIR/output-validation-report
fi
