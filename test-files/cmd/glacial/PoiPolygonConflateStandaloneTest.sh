#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/glacial/PoiPolygonConflateStandaloneTest

hoot conflate --warn -C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D uuid.helper.repeatable=true -D match.creators="hoot::PoiPolygonMatchCreator" -D merger.creators="hoot::PoiPolygonMergerCreator" -D poi.polygon.address.match.enabled=true -D address.scorer.enable.caching=true test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm test-output/cmd/glacial/PoiPolygonConflateStandaloneTest/output1.osm
hoot diff test-output/cmd/glacial/PoiPolygonConflateStandaloneTest/output1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/output1.osm || diff test-output/cmd/glacial/PoiPolygonConflateStandaloneTest/output1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/output1.osm
