#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/RailwayDiffConflateTest
OUT_DIR=test-output/cmd/slow/RailwayDiffConflateTest
mkdir -p $OUT_DIR
LOG_LEVEL=--warn
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf \
  -D uuid.helper.repeatable=true -D match.creators=ScriptMatchCreator,Railway.js \
  -D merger.creators=ScriptMergerCreator"

# See related notes in DiffConflatePartialLinearMatchTest.

# remove partial matches partially
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=false \
  -D conflate.pre.ops++=ReplaceTagVisitor -D replace.tag.visitor.match.tag="railway=Other" \
  -D replace.tag.visitor.replace.tag="railway=rail" \
  test-files/cmd/slow/RailwayConflateTest/RR_Ref1_ManuallyMatched.osm \
  test-files/cmd/slow/RailwayConflateTest/RR_Ref2_ManuallyMatched.osm $OUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm || diff $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm
if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUT_DIR/output-partial.osm \
    --report-output $OUT_DIR/output-partial-validation-report \
    --output $OUT_DIR/output-partial-validated.osm
  diff $IN_DIR/output-partial-validation-report $OUT_DIR/output-partial-validation-report
fi

# remove partial matches completely
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.linear.partial.matches.as.whole=true \
  -D conflate.pre.ops++=ReplaceTagVisitor -D replace.tag.visitor.match.tag="railway=Other" \
  -D replace.tag.visitor.replace.tag="railway=rail" \
  test-files/cmd/slow/RailwayConflateTest/RR_Ref1_ManuallyMatched.osm \
  test-files/cmd/slow/RailwayConflateTest/RR_Ref2_ManuallyMatched.osm $OUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm || diff $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm
if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUT_DIR/output-complete.osm \
    --report-output $OUT_DIR/output-complete-validation-report \
    --output $OUT_DIR/output-complete-validated.osm
  diff $IN_DIR/output-complete-validation-report $OUT_DIR/output-complete-validation-report
fi
