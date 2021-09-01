#!/bin/bash
set -e

# TEST_NAME=$1
# REF_INPUT=$2
# SEC_INPUT=$3
# BOUNDS=$4
# INCLUDE_TAGS=$5
# SEPARATE_OUTPUT=$6
# ALG_CONFIG=$7
# NUM_STEPS=$8
# WAY_SNAP_TOLERANCE=$9

test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off "ServiceDiffNetworkRoadSnap2Test" "test-files/cmd/slow/serial/ServiceDiffNetworkRoadSnap2Test/Input1.osm" "test-files/cmd/slow/serial/ServiceDiffNetworkRoadSnap2Test/Input2.osm" "-180,-90,180,90" "false" "false" "NetworkAlgorithm.conf" "3" "5.0"


