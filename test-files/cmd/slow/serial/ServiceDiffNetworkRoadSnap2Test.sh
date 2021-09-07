#!/bin/bash
set -e

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

TEST_NAME=ServiceDiffNetworkRoadSnap2Test
IN_DIR=test-files/cmd/slow/serial/$TEST_NAME
test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off $TEST_NAME "$IN_DIR/Input1.osm" "$IN_DIR/Input2.osm" $IN_DIR "test-output/cmd/slow/serial/$TEST_NAME" "-180,-90,180,90" "false" "false" "NetworkAlgorithm.conf" "3" "5.0" "false" "false"


