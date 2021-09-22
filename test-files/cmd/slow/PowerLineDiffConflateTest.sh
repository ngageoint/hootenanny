#!/bin/bash
set -e

# TODO: fix the orphaned power pole nodes in the output as part of #4465

IN_DIR=test-files/cmd/slow/PowerLineDiffConflateTest
IN_DIR_2=test-files/cmd/slow/PowerLineConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineDiffConflateTest
mkdir -p $OUT_DIR
LOG_LEVEL=--status
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf -D uuid.helper.repeatable=true -D match.creators=ScriptMatchCreator,PowerLine.js -D merger.creators=ScriptMergerCreator"

# See related notes in DiffConflatePartialLinearMatchTest.

# remove partial matches partially
TO_VALIDATE_1=test-output/cmd/slow/PowerLineDiffConflateTest/output-partial.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/PowerLineDiffConflateTest/output-partial-validation-report
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false \
  $IN_DIR_2/power-line-1.osm $IN_DIR_2/power-line-2.osm $OUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm || \
  diff $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm

# remove partial matches completely
TO_VALIDATE_2=test-output/cmd/slow/PowerLineDiffConflateTest/output-complete.osm
VALIDATION_REPORT_GOLD_2=test-files/cmd/slow/PowerLineDiffConflateTest/output-complete-validation-report
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=true \
  $IN_DIR_2/power-line-1.osm $IN_DIR_2/power-line-2.osm $OUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm || \
  diff $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm

