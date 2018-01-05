#!/bin/bash
set -e

REF_DIR=test-files/cmd/quick/DeriveRawImplicitTagRulesCmdTest
OUTPUT_DIR=test-output/cmd/quick/DeriveRawImplicitTagRulesCmdTest
INPUT_DIR=test-files/schema/DeriveRawImplicitTagRulesCmdTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot implicit-tagging-derive-raw-rules --warn poi "$INPUT_DIR/yemen-crop-2.osm.pbf;$INPUT_DIR/philippines-1.osm.pbf" "translations/OSM_Ingest.js;translations/OSM_Ingest.js" $OUTPUT_DIR/GenerateRawPoiImplicitTagRulesCmdTest-out.implicitTagRules

#only checking the json and tsv files here
diff $REF_DIR/DeriveRawImplicitTagRulesCmdTest.implicitTagRules $OUTPUT_DIR/DeriveRawImplicitTagRulesCmdTest-out.implicitTagRules
