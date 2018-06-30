#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/CountFeaturesCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/conflate/unified/AllDataTypesA.osm
INPUT_FILE_2=test-files/conflate/unified/AllDataTypesB.osm
INPUT_FILE_3=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm

hoot count-features --warn "$INPUT_FILE_1;$INPUT_FILE_2"
hoot count-features --warn "$INPUT_FILE_1;$INPUT_FILE_2" --all-elements
hoot count-features --warn "$INPUT_FILE_1;$INPUT_FILE_2" hoot::PoiCriterion
hoot count-features --warn -D element.criterion.negate=true "$INPUT_FILE_1;$INPUT_FILE_2" hoot::PoiCriterion
hoot count-features --warn $INPUT_FILE_3 hoot::CountManualMatchesVisitor
