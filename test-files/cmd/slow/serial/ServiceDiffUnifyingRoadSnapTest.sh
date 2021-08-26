#!/bin/bash
set -e

# TEST_NAME=$1
# REF_INPUT=$2
# SEC_INPUT=$3
# BOUNDS=$4
# INCLUDE_TAGS=$5
# SEPARATE_OUPUT=$6
# ALG_CONFIG=$7
# NUM_STEPS=$8

test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off "ServiceDiffUnifyingRoadSnapTest" "test-files/cmd/glacial/DiffConflateCmdTest/input3.osm" "test-files/cmd/glacial/DiffConflateCmdTest/input4.osm" "-180,-90,180,90" "false" "false" "UnifyingAlgorithm.conf" "2"


