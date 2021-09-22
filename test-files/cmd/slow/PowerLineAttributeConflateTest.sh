#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/PowerLineAttributeConflateTest
IN_DIR_2=test-files/cmd/slow/PowerLineConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

TO_VALIDATE_1=test-output/cmd/slow/PowerLineAttributeConflateTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/PowerLineAttributeConflateTest/output-validation-report
hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf \
  -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,PowerLine.js" \
  -D merger.creators="ScriptMergerCreator" $IN_DIR_2/power-line-1.osm $IN_DIR_2/power-line-2.osm \
  $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
