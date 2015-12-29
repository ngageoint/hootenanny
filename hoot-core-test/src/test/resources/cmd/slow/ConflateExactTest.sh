#!/bin/bash
set -e

mkdir -p test-output/cmd/ConlateExactTest
STATS_OUT=test-output/cmd/ConlateExactTest/boston-road-building-out

hoot conflate -D writer.include.debug=true test-files/BostonSubsetRoadBuilding_FromOsm.osm test-files/BostonSubsetRoadBuilding_FromOsm.osm $STATS_OUT.osm --stats > $STATS_OUT

hoot is-match $STATS_OUT.osm test-files/cmd/slow/ConflateExactTest/output.osm
