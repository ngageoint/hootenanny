#!/bin/bash
set -e

# rnd test
OPT_COMMAND=`hoot | grep type-tagger-create-rules`
if [ -z "$OPT_COMMAND" ]; then
 exit 0
fi

REF_DIR=test-files/cmd/slow/ImplicitTypeTaggerTest
OUTPUT_DIR=test-output/cmd/slow/ImplicitTypeTaggerTest
INPUT_DIR=test-files/schema/ImplicitTagRawRulesDeriverTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot type-tagger-create-rules --warn "$INPUT_DIR/yemen-crop-2.osm.pbf;$INPUT_DIR/philippines-1.osm.pbf" "translations/OSM_Ingest.js;translations/OSM_Ingest.js" $OUTPUT_DIR/ImplicitTypeTaggerTest-out.implicitTagRules
diff $REF_DIR/ImplicitTypeTaggerTest.implicitTagRules $OUTPUT_DIR/ImplicitTypeTaggerTest-out.implicitTagRules

hoot type-tagger-create-rules-db --warn $OUTPUT_DIR/ImplicitTypeTaggerTest-out.implicitTagRules "$OUTPUT_DIR/ImplicitTypeTaggerTest-out.sqlite"

hoot type-tagger-create-rules-db "$OUTPUT_DIR/ImplicitTypeTaggerTest-out.sqlite" --stats
