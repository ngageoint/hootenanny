#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/conflate/unified
OUT_DIR=test-output/cmd/slow/ConflateCmdTest
mkdir -p $OUT_DIR
OUT_FILE=$OUT_DIR/output.osm

# TODO
TO_VALIDATE=$OUT_FILE
VALIDATION_REPORT_GOLD=test-files/cmd/slow/ConflateCmdTest/validation-report

export CONFLATE_OPTS="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf -C Testing.conf"

hoot conflate $CONFLATE_OPTS $IN_DIR/AllDataTypesA.osm $IN_DIR/AllDataTypesB.osm $OUT_DIR/output.osm
hoot diff -C Testing.conf $OUT_DIR/output.osm $IN_DIR/output.osm || diff $OUT_DIR/output.osm $IN_DIR/output.osm

# Check to make sure we don't bomb out on empty files
hoot conflate $CONFLATE_OPTS --warn test-files/Empty.osm test-files/Empty.osm tmp/ConflateCmdTest.osm
hoot diff -C Testing.conf test-files/Empty.osm tmp/ConflateCmdTest.osm || cat tmp/ConflateCmdTest.osm
