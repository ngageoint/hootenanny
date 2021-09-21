#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/conflate/unified
IN_DIR_2=test-files/cmd/slow/ConflateCmdTest
OUT_DIR=test-output/cmd/slow/ConflateCmdTest
mkdir -p $OUT_DIR
OUT_FILE=$OUT_DIR/output.osm

export CONFLATE_OPTS="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf -C Testing.conf"

# TODO
TO_VALIDATE_1=test-output/cmd/slow/ConflateCmdTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/ConflateCmdTest/output-validation-report

hoot conflate $CONFLATE_OPTS $IN_DIR/AllDataTypesA.osm $IN_DIR/AllDataTypesB.osm $OUT_DIR/output.osm
hoot diff -C Testing.conf $OUT_DIR/output.osm $IN_DIR_2/output.osm || diff $OUT_DIR/output.osm $IN_DIR_2/output.osm

# Check to make sure we don't bomb out on empty files
hoot conflate $CONFLATE_OPTS --warn test-files/Empty.osm test-files/Empty.osm $OUT_DIR/empty-out.osm
hoot diff -C Testing.conf test-files/Empty.osm $OUT_DIR/empty-out.osm || cat $OUT_DIR/empty-out.osm
