#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/PowerLineDifferentialConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineDifferentialConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C DifferentialConflation.conf -D differential.remove.linear.partial.matches.as.whole=false -D uuid.helper.repeatable=true -D match.creators="hoot::ScriptMatchCreator,PowerLine.js" -D merger.creators="hoot::ScriptMergerCreator" test-files/cmd/glacial/PowerLineConflateTest/power-line-1.osm test-files/cmd/glacial/PowerLineConflateTest/power-line-2.osm $OUT_DIR/output.osm --differential
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
