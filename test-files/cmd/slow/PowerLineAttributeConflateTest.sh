#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/PowerLineAttributeConflateTest
OUT_DIR=test-output/cmd/slow/PowerLineAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,PowerLine.js" -D merger.creators="ScriptMergerCreator" test-files/cmd/slow/PowerLineConflateTest/power-line-1.osm test-files/cmd/slow/PowerLineConflateTest/power-line-2.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
