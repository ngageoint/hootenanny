#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/ConflateInlineTranslationTest
OUT_DIR=test-output/cmd/slow/ConflateInlineTranslationTest
rm -rf $OUT_DIR
mkdir -p $OUT_DIR

CONFIG="-C Testing.conf -C AttributeConflation.conf -C UnifyingAlgorithm.conf"
LOG_LEVEL="--warn"

# Note that 
hoot conflate $LOG_LEVEL $CONFIG -D conflate.pre.ops++="SchemaTranslationOp" -D schema.translation.script=$IN_DIR/translation-script.js -D schema.translation.element.status=Unknown1 $IN_DIR/input1.osm $IN_DIR/input2.osm $OUT_DIR/out.osm
hoot diff $CONFIG $LOG_LEVEL $IN_DIR/out.osm $OUT_DIR/out.osm
