#!/bin/bash
set -e

REF_DIR=test-files/cmd/slow/RndImplicitTypeDatabaseDeriverTest
OUTPUT_DIR=test-output/cmd/slow/RndImplicitTypeDatabaseDeriverTest
INPUT_DIR=test-files/schema/ImplicitTagRawRulesDeriverTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot type-tagger-create-rules --warn "$INPUT_DIR/yemen-crop-2.osm.pbf;$INPUT_DIR/philippines-1.osm.pbf" "translations/OSM_Ingest.js;translations/OSM_Ingest.js" $OUTPUT_DIR/RndImplicitTypeDatabaseDeriverTest-out.implicitTagRules
diff $REF_DIR/RndImplicitTypeDatabaseDeriverTest.implicitTagRules $OUTPUT_DIR/RndImplicitTypeDatabaseDeriverTest-out.implicitTagRules

hoot type-tagger-create-rules-db --warn $OUTPUT_DIR/RndImplicitTypeDatabaseDeriverTest-out.implicitTagRules "$OUTPUT_DIR/RndImplicitTypeDatabaseDeriverTest-out.sqlite"
hoot type-tagger-create-rules-db "$OUTPUT_DIR/RndImplicitTypeDatabaseDeriverTest-out.sqlite" --stats
