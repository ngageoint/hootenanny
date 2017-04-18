#!/bin/bash
set -e

mkdir -p test-output/cmd/ConflateExactTest
STATS_OUT=test-output/cmd/ConflateExactTest/boston-road-building-out

hoot conflate -D writer.include.debug.tags=true test-files/BostonSubsetRoadBuilding_FromShp.osm test-files/BostonSubsetRoadBuilding_FromOsm.osm $STATS_OUT.osm > $STATS_OUT

hoot is-match $STATS_OUT.osm test-files/cmd/slow/ConflateExactTest/output.osm
