#!/bin/bash
set -e

test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off "ServiceDiffNetworkRoadSnapTest" "test-files/cmd/slow/DiffConflateCmdTest/input3.osm" "test-files/cmd/slow/DiffConflateCmdTest/input4.osm" "NetworkAlgorithm.conf" "2"


