#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/NetworkConflateCmdRelationEdgeTest
OUTPUT_DIR=test-output/cmd/slow/NetworkConflateCmdRelationEdgeTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

# The purpose of this test is to catch make sure we're preventing an error when the Network Roads Alg tries to calculate a heading at a vertex
# with a relation as the network edge input. It should skip it unless the edge is a way.
hoot conflate --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/out.osm
hoot diff $INPUT_DIR/out.osm $OUTPUT_DIR/out.osm || diff $INPUT_DIR/out.osm $OUTPUT_DIR/out.osm
