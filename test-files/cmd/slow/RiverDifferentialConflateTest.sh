#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/RiverDifferentialConflateTest
OUT_DIR=test-output/cmd/slow/RiverDifferentialConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C DifferentialConflation.conf -D differential.remove.river.partial.matches.as.whole=false -D uuid.helper.repeatable=true -D match.creators="hoot::ScriptMatchCreator,River.js" -D merger.creators="hoot::ScriptMergerCreator" test-files/conflate/generic/rivers/Haiti_CNIGS_Rivers_REF1-cropped-2.osm test-files/conflate/generic/rivers/Haiti_osm_waterway_ss_REF2-cropped-2.osm $OUT_DIR/output.osm --differential
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
