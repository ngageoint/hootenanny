#!/bin/bash
set -e

REF_DIR=test-files/cmd/quick/DeriveImplicitTagRulesDatabaseCmdTest
OUTPUT_DIR=test-output/cmd/quick/DeriveImplicitTagRulesDatabaseCmdTest
INPUT_DIR=test-files/schema/ImplicitTagRulesDeriverTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot implicit-tagging-derive-raw-rules --warn poi $INPUT_DIR/PoiImplicitTagRulesDeriverTest-input.implicitTagRules "$OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.json;$OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.tsv;$OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.sqlite"

#only checking the json and tsv files here
#diff $REF_DIR/DerivePoiImplicitTagRulesCmdTest.json $OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.json
#diff $REF_DIR/DerivePoiImplicitTagRulesCmdTest.tsv $OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.tsv
 
