#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/RiverAttributeConflateTest
IN_DIR_2=test-files/conflate/generic/rivers
OUT_DIR=test-output/cmd/slow/RiverAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf \
  -D "match.creators=ScriptMatchCreator,River.js" -D "merger.creators=ScriptMergerCreator" \
  -D writer.include.debug.tags=true $IN_DIR_2/Haiti_CNIGS_Rivers_REF1-cropped-2.osm \
  $IN_DIR_2/Haiti_osm_waterway_ss_REF2-cropped-2.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm

if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $CONFIG $OUT_DIR/output.osm \
    --report-output $OUT_DIR/output-validation-report --output $OUT_DIR/output-validated.osm
  diff $IN_DIR/output-validation-report $OUT_DIR/output-validation-report
fi

