#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/glacial/PoiPolygonConflateCombinedTest

# Making this info instead of warning output to catch a situation where a generic script was calculating the search radius multiple times.
hoot conflate -C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D uuid.helper.repeatable=true -D poi.polygon.address.match.enabled=true test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm test-output/cmd/glacial/PoiPolygonConflateCombinedTest/output1.osm
hoot diff test-output/cmd/glacial/PoiPolygonConflateCombinedTest/output1.osm test-files/cmd/glacial/PoiPolygonConflateCombinedTest/output1.osm || diff test-output/cmd/glacial/PoiPolygonConflateCombinedTest/output1.osm test-files/cmd/glacial/PoiPolygonConflateCombinedTest/output1.osm
