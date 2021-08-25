#!/bin/bash
set -e

# This tests that certain ref roads aren't unnecessarily deleted upon changeset application.
# Particularly, we're looking at the road with REF1=0008fd.

# TEST_NAME=$1
# REF_INPUT=$2
# SEC_INPUT=$3
# BOUNDS=$4
# INCLUDE_TAGS=$5
# SEPARATE_OUPUT=$6
# ALG_CONFIG=$7
# NUM_STEPS=$8

IN_DIR=test-files/cmd/slow/serial/ServiceDiffDroppedRefRoad4977Test
test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off "ServiceDiffDroppedRefRoad4977Test" "$IN_DIR/input1.osm" "$IN_DIR/input2.osm" "-76.404934,-13.054061;-76.342314,-13.053018;-76.341627,-13.092146;-76.404247,-13.093189;-76.404934,-13.054061" "true" "true" "UnifyingAlgorithm.conf" "2"


