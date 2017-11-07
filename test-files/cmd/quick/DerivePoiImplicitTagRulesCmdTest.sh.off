#!/bin/bash
set -e

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep derive-poi-implicit-tag-rules`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "multiary-ingest command not present.  Exiting test without running it."
 exit 0
fi

REF_DIR=test-files/cmd/quick/DerivePoiImplicitTagRulesCmdTest
OUTPUT_DIR=test-output/cmd/quick/DerivePoiImplicitTagRulesCmdTest
INPUT_DIR=test-files/io/PoiImplicitTagRulesDeriverTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot derive-poi-implicit-tag-rules --warn "$INPUT_DIR/yemen-crop-2.osm.pbf;$INPUT_DIR/philippines-1.osm.pbf" "translations/OSM_Ingest.js;translations/OSM_Ingest.js" "$OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.json;$OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.tsv;$OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.sqlite" "amenity;tourism;building" 4

#only checking the json and tsv files here
diff $REF_DIR/DerivePoiImplicitTagRulesCmdTest.json $OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.json
diff $REF_DIR/DerivePoiImplicitTagRulesCmdTest.tsv $OUTPUT_DIR/DerivePoiImplicitTagRulesCmdTest-out.tsv
 
