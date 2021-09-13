#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/RailwayConflateTest
OUT_DIR=test-output/cmd/slow/RailwayConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,Railway.js" -D merger.creators="ScriptMergerCreator" -D conflate.pre.ops++="ReplaceTagVisitor" -D replace.tag.visitor.match.tag="railway=Other" -D replace.tag.visitor.replace.tag="railway=rail" $IN_DIR/RR_Ref1_ManuallyMatched.osm $IN_DIR/RR_Ref2_ManuallyMatched.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
