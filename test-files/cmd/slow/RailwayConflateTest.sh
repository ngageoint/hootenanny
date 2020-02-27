#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/glacial/RailwayConflateTest
OUT_DIR=test-output/cmd/glacial/RailwayConflateTest
mkdir -p test-output/cmd/glacial/RailwayConflateTest

CONFIG="-C Testing.conf"

hoot conflate --warn $CONFIG -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D uuid.helper.repeatable=true -D match.creators="hoot::ScriptMatchCreator,Railway.js" -D merger.creators="hoot::ScriptMergerCreator" $IN_DIR/RR_Ref1_ManuallyMatched.osm $IN_DIR/RR_Ref2_ManuallyMatched.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm
