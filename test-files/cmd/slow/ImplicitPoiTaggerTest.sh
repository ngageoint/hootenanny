#!/bin/bash
set -e

REF_DIR=test-files/cmd/slow/ImplicitPoiTaggerTest
OUTPUT_DIR=test-output/cmd/slow/ImplicitPoiTaggerTest
INPUT_DIR=test-files/schema/ImplicitTagRawRulesDeriverTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot type-tagger-create-rules --warn poi "$INPUT_DIR/yemen-crop-2.osm.pbf;$INPUT_DIR/philippines-1.osm.pbf" "translations/OSM_Ingest.js;translations/OSM_Ingest.js" $OUTPUT_DIR/ImplicitPoiTaggerTest-out.implicitTagRules
diff $REF_DIR/ImplicitPoiTaggerTest.implicitTagRules $OUTPUT_DIR/ImplicitPoiTaggerTest-out.implicitTagRules

hoot type-tagger-create-rules-db --warn poi $OUTPUT_DIR/ImplicitPoiTaggerTest-out.implicitTagRules "$OUTPUT_DIR/ImplicitPoiTaggerTest-out.sqlite"

hoot type-tagger-create-rules-db "$OUTPUT_DIR/ImplicitPoiTaggerTest-out.sqlite" --stats
