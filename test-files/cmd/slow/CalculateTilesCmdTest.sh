#!/bin/bash
set -e

mkdir -p test-output/cmd/CalculateTilesCmdTest 

hoot calculate-tiles "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" test-output/cmd/CalculateTilesCmdTest/calc-tiles-out 1000 0.001

