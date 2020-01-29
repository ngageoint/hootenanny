#!/bin/bash
set -e

REF_DIR=test-files/cmd/glacial/ImplicitTypeDatabaseDeriverTest
OUTPUT_DIR=test-output/cmd/glacial/ImplicitTypeDatabaseDeriverTest
INPUT_DIR=test-files/schema/ImplicitTagRawRulesDeriverTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

HOOT_OPTS="-D language.translation.translator=hoot::ToEnglishDictionaryTranslator -D implicit.tagging.database.deriver.translate.names.to.english=true"

hoot type-tagger-rules --warn $HOOT_OPTS --create-raw "$INPUT_DIR/yemen-crop-2.osm.pbf;$INPUT_DIR/philippines-1.osm.pbf" "translations/OSM_Ingest.js;translations/OSM_Ingest.js" $OUTPUT_DIR/ImplicitTypeDatabaseDeriverTest-out.implicitTagRules
diff $REF_DIR/ImplicitTypeDatabaseDeriverTest.implicitTagRules $OUTPUT_DIR/ImplicitTypeDatabaseDeriverTest-out.implicitTagRules

hoot type-tagger-rules --warn $HOOT_OPTS --create-db $OUTPUT_DIR/ImplicitTypeDatabaseDeriverTest-out.implicitTagRules "$OUTPUT_DIR/ImplicitTypeDatabaseDeriverTest-out.sqlite"
hoot type-tagger-rules --db-stats "$OUTPUT_DIR/ImplicitTypeDatabaseDeriverTest-out.sqlite"
