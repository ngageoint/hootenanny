#!/bin/bash
set -e

test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off "ServiceDiffUnifyingRoadSnapTest" "test-files/cmd/glacial/DiffConflateCmdTest/input3.osm" "test-files/cmd/glacial/DiffConflateCmdTest/input4.osm" "UnifyingAlgorithm.conf" "2"


