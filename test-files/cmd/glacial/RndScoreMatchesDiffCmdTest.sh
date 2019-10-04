#!/bin/bash
set -e

IN_DIR=test-files/cmd/glacial/ScoreMatchesDiffCmdTest
OUT_DIR=test-output/cmd/glacial/ScoreMatchesDiffCmdTest
mkdir -p $OUT_DIR

hoot score-matches-diff --warn $IN_DIR/input1.osm $IN_DIR/input2.osm $OUT_DIR/output
diff $IN_DIR/output $OUT_DIR/output
