#!/bin/bash
set -e

test-files/cmd/slow/serial/ServiceDiffRoadSnapTest.sh.off "ServiceDiffNetworkRoadSnap2Test" "test-files/cmd/slow/serial/ServiceDiffNetworkRoadSnap2Test/Input1.osm" "test-files/cmd/slow/serial/ServiceDiffNetworkRoadSnap2Test/Input2.osm" "NetworkAlgorithm.conf" "3"


