#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/RiverAttributeConflateTest
OUT_DIR=test-output/cmd/slow/RiverAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="-C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf -D "match.creators=hoot::ScriptMatchCreator,River.js" -D "merger.creators=hoot::ScriptMergerCreator" -D writer.include.debug.tags=true test-files/conflate/generic/rivers/Haiti_CNIGS_Rivers_REF1-cropped-2.osm test-files/conflate/generic/rivers/Haiti_osm_waterway_ss_REF2-cropped-2.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm

