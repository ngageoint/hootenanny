#!/bin/bash
set -e

# derived from train-poi-poly-standalone regression test C cropped to -122.42035,37.7726,-122.41369,37.78593
# the first input was created running poi/poly with default options; the second raises the review threshold 
# (poi.polygon.review.evidence.threshold) to 2 to force some incorrect matches

IN_DIR=test-files/cmd/quick/ScoreMatchesDiffCmdTest
OUT_DIR=test-output/cmd/quick/ScoreMatchesDiffCmdTest
mkdir -p $OUT_DIR

hoot score-matches-diff --warn -C Testing.conf $IN_DIR/input1.osm $IN_DIR/input2.osm $OUT_DIR/output
diff $IN_DIR/output $OUT_DIR/output
