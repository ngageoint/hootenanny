#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/PoiPolygonConflateStandaloneTest

hoot conflate --warn -D uuid.helper.repeatable=true -D match.creators="hoot::PoiPolygonMatchCreator" -D merger.creators="hoot::PoiPolygonMergerCreator" test-files/cmd/slow/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/slow/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm test-output/cmd/PoiPolygonConflateStandaloneTest/output1.osm
hoot is-match test-output/cmd/PoiPolygonConflateStandaloneTest/output1.osm test-files/cmd/slow/PoiPolygonConflateStandaloneTest/output1.osm || diff test-output/cmd/PoiPolygonConflateStandaloneTest/output1.osm test-files/cmd/slow/PoiPolygonConflateStandaloneTest/output1.osm
