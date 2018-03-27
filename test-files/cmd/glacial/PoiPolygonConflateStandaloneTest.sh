#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/PoiPolygonConflateStandaloneTest

hoot conflate --warn -D uuid.helper.repeatable=true -D match.creators="hoot::PoiPolygonMatchCreator" -D merger.creators="hoot::PoiPolygonMergerCreator" test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm test-output/cmd/PoiPolygonConflateStandaloneTest/output1.osm
hoot map-diff test-output/cmd/PoiPolygonConflateStandaloneTest/output1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/output1.osm || diff test-output/cmd/PoiPolygonConflateStandaloneTest/output1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/output1.osm
