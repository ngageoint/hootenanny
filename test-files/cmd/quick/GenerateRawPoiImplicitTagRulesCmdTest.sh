#!/bin/bash
set -e

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep generate-raw-poi-implicit-tag-rules`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "generate-raw-poi-implicit-tag-rules command not present.  Exiting test without running it."
 exit 0
fi

REF_DIR=test-files/cmd/quick/GenerateRawPoiImplicitTagRulesCmdTest
OUTPUT_DIR=test-output/cmd/quick/GenerateRawPoiImplicitTagRulesCmdTest
INPUT_DIR=test-files/schema/PoiImplicitTagRawRulesGeneratorTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot generate-raw-poi-implicit-tag-rules --warn "$INPUT_DIR/yemen-crop-2.osm.pbf;$INPUT_DIR/philippines-1.osm.pbf" "translations/OSM_Ingest.js;translations/OSM_Ingest.js" $OUTPUT_DIR/GenerateRawPoiImplicitTagRulesCmdTest-out.implicitTagRules

#only checking the json and tsv files here
diff $REF_DIR/GenerateRawPoiImplicitTagRulesCmdTest.implicitTagRules $OUTPUT_DIR/GenerateRawPoiImplicitTagRulesCmdTest-out.implicitTagRules
