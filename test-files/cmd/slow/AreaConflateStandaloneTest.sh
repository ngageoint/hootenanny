#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/AreaConflateStandaloneTest

hoot conflate --warn -D uuid.helper.repeatable=true -D match.creators="hoot::ScriptMatchCreator,Area.js" -D merger.creators="hoot::ScriptMergerCreator" test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm test-output/cmd/slow/AreaConflateStandaloneTest/output1.osm
hoot diff test-output/cmd/slow/AreaConflateStandaloneTest/output1.osm test-files/cmd/slow/AreaConflateStandaloneTest/output1.osm || diff test-output/cmd/slow/AreaConflateStandaloneTest/output1.osm test-files/cmd/slow/AreaConflateStandaloneTest/output1.osm
