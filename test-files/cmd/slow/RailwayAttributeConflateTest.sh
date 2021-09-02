#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/RailwayAttributeConflateTest
OUT_DIR=test-output/cmd/slow/RailwayAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,Railway.js" -D merger.creators="ScriptMergerCreator" test-files/cmd/slow/RailwayConflateTest/RR_Ref1_ManuallyMatched.osm test-files/cmd/slow/RailwayConflateTest/RR_Ref2_ManuallyMatched.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
