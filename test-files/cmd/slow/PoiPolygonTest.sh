#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/PoiPolygonTest

#standalone
hoot conflate -D match.creators="hoot::PoiPolygonMatchCreator" -D merger.creators="hoot::PoiPolygonMergerCreator" test-files/cmd/slow/PoiPolygonTest/PoiPolygon1.osm test-files/cmd/slow/PoiPolygonTest/PoiPolygon2.osm test-output/cmd/PoiPolygonTest/output1.osm
hoot is-match test-output/cmd/PoiPolygonTest/output1.osm test-files/cmd/slow/PoiPolygonTest/output1.osm || diff test-output/cmd/PoiPolygonTest/output1.osm test-files/cmd/slow/PoiPolygonTest/output1.osm

#combined
hoot conflate test-files/cmd/slow/PoiPolygonTest/PoiPolygon1.osm test-files/cmd/slow/PoiPolygonTest/PoiPolygon2.osm test-output/cmd/PoiPolygonTest/output2.osm
hoot is-match test-output/cmd/PoiPolygonTest/output2.osm test-files/cmd/slow/PoiPolygonTest/output2.osm || diff test-output/cmd/PoiPolygonTest/output2.osm test-files/cmd/slow/PoiPolygonTest/output2.osm
