#!/bin/bash
set -e

IN_DIR=test-files/conflate/unified
IN_DIR_2=test-files/cmd/slow/ConflateCmdTest
OUT_DIR=test-output/cmd/slow/ConflateCmdTest
mkdir -p $OUT_DIR

CONFIG="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf -C Testing.conf"
source scripts/core/ScriptTestUtils.sh

hoot conflate $CONFIG $IN_DIR/AllDataTypesA.osm $IN_DIR/AllDataTypesB.osm $OUT_DIR/output.osm
hoot diff -C Testing.conf $OUT_DIR/output.osm $IN_DIR_2/output.osm || diff $OUT_DIR/output.osm $IN_DIR_2/output.osm
validateTestOutput $OUT_DIR/output.osm $OUT_DIR/output-validation-report \
  $OUT_DIR/output-validated.osm $IN_DIR_2/output-validation-report

# Check to make sure we don't bomb out on empty files
hoot conflate $CONFIG --warn test-files/Empty.osm test-files/Empty.osm $OUT_DIR/empty-out.osm
hoot diff -C Testing.conf test-files/Empty.osm $OUT_DIR/empty-out.osm || cat $OUT_DIR/empty-out.osm
