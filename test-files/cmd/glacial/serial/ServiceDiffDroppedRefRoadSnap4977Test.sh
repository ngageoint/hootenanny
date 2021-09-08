#!/bin/bash
set -e

# This tests that certain ref roads aren't unnecessarily deleted upon changeset application.
# Particularly, we're looking at the roads with REF1=0008fd and REF1=000a12.
#
# main test AOI: -76.404934,-13.054061;-76.342314,-13.053018;-76.341627,-13.092146;-76.404247,-13.093189;-76.404934,-13.054061
# test AOI for first two unsnapped roads fixed in 4977: -76.63858,-13.0774,-76.3679,-13.0659

# TEST_NAME=$1
# REF_INPUT=$2
# SEC_INPUT=$3
# GOLD_DIR=$4
# OUTPUT_DIR=$5
# BOUNDS=$6
# INCLUDE_TAGS=$7
# SEPARATE_OUTPUT=$8
# ALG_CONFIG=$9
# NUM_STEPS=${10}
# WAY_SNAP_TOLERANCE=${11}
# CONFLATE_FROM_FILE=${12}
# UNIFYING_RUBBERSHEET=${13}

TEST_NAME=ServiceDiffDroppedRefRoadSnap4977Test
IN_DIR=test-files/cmd/glacial/serial/$TEST_NAME
test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off $TEST_NAME "$IN_DIR/input1.osm" "$IN_DIR/input2.osm" $IN_DIR "test-output/cmd/glacial/serial/$TEST_NAME" "-76.404934,-13.054061;-76.342314,-13.053018;-76.341627,-13.092146;-76.404247,-13.093189;-76.404934,-13.054061" "true" "true" "UnifyingAlgorithm.conf" "2" "5.0" "false" "true"


