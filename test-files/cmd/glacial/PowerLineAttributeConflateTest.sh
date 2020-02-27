#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/glacial/PowerLineConflateTest
OUT_DIR=test-output/cmd/glacial/PowerLineAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="-C Testing.conf"

hoot conflate --warn $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf -D uuid.helper.repeatable=true -D match.creators="hoot::ScriptMatchCreator,PowerLine.js" -D merger.creators="hoot::ScriptMergerCreator" $IN_DIR/power-line-1.osm $IN_DIR/power-line-2.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
