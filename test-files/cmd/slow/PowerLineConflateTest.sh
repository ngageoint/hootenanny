#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/PowerLineConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineConflateTest
mkdir -p $OUT_DIR

CONFIG="-C Testing.conf"

TO_VALIDATE_1=test-output/cmd/slow/PowerLineConflateTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/PowerLineConflateTest/output-validation-report
hoot conflate --warn $CONFIG -C UnifyingAlgorithm.conf -C ReferenceConflation.conf \
  -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,PowerLine.js" \
  -D merger.creators="ScriptMergerCreator" $IN_DIR/power-line-1.osm $IN_DIR/power-line-2.osm \
  $OUT_DIR/output.osm
hoot diff $CONFIG $OUT_DIR/output.osm $IN_DIR/output.osm || diff $OUT_DIR/output.osm $IN_DIR/output.osm
