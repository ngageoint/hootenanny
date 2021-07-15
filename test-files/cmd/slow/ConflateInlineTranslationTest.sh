#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/ConflateInlineTranslationTest
OUT_DIR=test-output/cmd/slow/ConflateInlineTranslationTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

CONFIG="-C Testing.conf -C AttributeConflation.conf -C UnifyingAlgorithm.conf"
LOG_LEVEL="--warn"

# -D schema.translation.direction=toosm
hoot conflate $LOG_LEVEL $CONFIG -D conflate.pre.ops++="SchemaTranslationOp" \
  -D schema.translation.script=$IN_DIR/translation-script.js \ 
  $IN_DIR/input1.osm $IN_DIR/input2.osm $OUT_DIR/out.osm
hoot diff $CONFIG $LOG_LEVEL $INPUT_DIR/out.osm $OUTPUT_DIR/out.osm
