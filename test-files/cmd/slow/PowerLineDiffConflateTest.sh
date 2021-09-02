#!/bin/bash
set -e

# TODO: fix the orphaned power pole nodes in the output as part of #4465

IN_DIR=test-files/cmd/slow/PowerLineDiffConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineDiffConflateTest
mkdir -p $OUT_DIR
LOG_LEVEL=--warn
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf -D uuid.helper.repeatable=true -D match.creators=ScriptMatchCreator,PowerLine.js -D merger.creators=ScriptMergerCreator"

# See related notes in DiffConflatePartialLinearMatchTest.

# remove partial matches partially
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false  test-files/cmd/slow/PowerLineConflateTest/power-line-1.osm test-files/cmd/slow/PowerLineConflateTest/power-line-2.osm $OUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm || diff $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm

# remove partial matches completely
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=true test-files/cmd/slow/PowerLineConflateTest/power-line-1.osm test-files/cmd/slow/PowerLineConflateTest/power-line-2.osm $OUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm || diff $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm

