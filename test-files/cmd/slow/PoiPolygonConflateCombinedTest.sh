#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/PoiPolygonConflateCombinedTest

# Making this info instead of warning output to catch a situation where a generic script was calculating the search radius multiple times.
hoot conflate -D uuid.helper.repeatable=true test-files/cmd/slow/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/slow/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm test-output/cmd/PoiPolygonConflateCombinedTest/output1.osm
hoot is-match test-output/cmd/PoiPolygonConflateCombinedTest/output1.osm test-files/cmd/slow/PoiPolygonConflateCombinedTest/output1.osm || diff test-output/cmd/PoiPolygonConflateCombinedTest/output1.osm test-files/cmd/slow/PoiPolygonConflateCombinedTest/output1.osm
