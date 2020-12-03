#!/bin/bash
set -e

test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off "ServiceDiffUnifyingRoadSnapTest" "test-files/cmd/slow/DiffConflateCmdTest/input3.osm" "test-files/cmd/slow/DiffConflateCmdTest/input4.osm" "UnifyingAlgorithm.conf" "2"


