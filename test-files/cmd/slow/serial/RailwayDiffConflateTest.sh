#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/serial/RailwayDiffConflateTest
IN_DIR_2=test-files/cmd/slow/serial/RailwayConflateTest
OUT_DIR=test-output/cmd/slow/serial/RailwayDiffConflateTest
mkdir -p $OUT_DIR
LOG_LEVEL=--warn
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf \
  -D uuid.helper.repeatable=true -D match.creators=ScriptMatchCreator,Railway.js \
  -D merger.creators=ScriptMergerCreator"
source scripts/core/ScriptTestUtils.sh

# See related notes in DiffConflatePartialLinearMatchTest.

# remove partial matches partially
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false \
  -D conflate.pre.ops++=ReplaceTagVisitor -D replace.tag.visitor.match.tag="railway=Other" \
  -D replace.tag.visitor.replace.tag="railway=rail" $IN_DIR_2/RR_Ref1_ManuallyMatched.osm \
  $IN_DIR_2/RR_Ref2_ManuallyMatched.osm $OUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm || \
  diff $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm
validateTestOutput $OUT_DIR/output-partial.osm $OUT_DIR/output-partial-validation-report \
  $OUT_DIR/output-partial-validated.osm $IN_DIR/output-partial-validation-report

# remove partial matches completely
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=true \
  -D conflate.pre.ops++=ReplaceTagVisitor -D replace.tag.visitor.match.tag="railway=Other" \
  -D replace.tag.visitor.replace.tag="railway=rail" $IN_DIR_2/RR_Ref1_ManuallyMatched.osm \
  $IN_DIR_2/RR_Ref2_ManuallyMatched.osm $OUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm || \
  diff $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm
validateTestOutput $OUT_DIR/output-complete.osm $OUT_DIR/output-complete-validation-report \
  $OUT_DIR/output-complete-validated.osm $IN_DIR/output-complete-validation-report
