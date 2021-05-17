#!/bin/bash
set -e

# Note that differential.remove.river.partial.matches.as.whole overrides differential.remove.linear.partial.matches.as.whole.

IN_DIR=test-files/cmd/slow/RiverDiffConflateTest
OUT_DIR=test-output/cmd/slow/RiverDiffConflateTest
mkdir -p $OUT_DIR
LOG_LEVEL=--warn
CONFIG="-C UnifyingAlgorithm.conf -C DifferentialConflation.conf -C Testing.conf -D uuid.helper.repeatable=true -D match.creators=hoot::ScriptMatchCreator,River.js -D merger.creators=hoot::ScriptMergerCreator"

# See related notes in DiffConflatePartialLinearMatchTest.

# remove partial matches partially
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.river.partial.matches.as.whole=false test-files/conflate/generic/rivers/Haiti_CNIGS_Rivers_REF1-cropped-2.osm test-files/conflate/generic/rivers/Haiti_osm_waterway_ss_REF2-cropped-2.osm $OUT_DIR/output-partial.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm || diff $IN_DIR/output-partial.osm $OUT_DIR/output-partial.osm

# remove partial matches completely
hoot conflate $LOG_LEVEL $CONFIG -D differential.remove.river.partial.matches.as.whole=true test-files/conflate/generic/rivers/Haiti_CNIGS_Rivers_REF1-cropped-2.osm test-files/conflate/generic/rivers/Haiti_osm_waterway_ss_REF2-cropped-2.osm $OUT_DIR/output-complete.osm --differential
hoot diff $LOG_LEVEL -C Testing.conf $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm || diff $IN_DIR/output-complete.osm $OUT_DIR/output-complete.osm

