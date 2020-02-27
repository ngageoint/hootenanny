#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/AreaAttributeConflateStandaloneTest
OUT_DIR=test-output/cmd/slow/AreaAttributeConflateStandaloneTest
mkdir -p $OUT_DIR

hoot conflate --warn -C Testing.conf -C UnifyingAlgorithm.conf -C AttributeConflation.conf -D uuid.helper.repeatable=true -D match.creators="hoot::ScriptMatchCreator,Area.js" -D merger.creators="hoot::ScriptMergerCreator" test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm $OUT_DIR/output.osm
hoot diff $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
