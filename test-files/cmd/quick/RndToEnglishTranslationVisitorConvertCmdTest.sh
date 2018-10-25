#!/bin/bash
set -e

INPUT_DIR=test-files/visitors/ToEnglishTranslationVisitorTest
INPUT_FILE=$INPUT_DIR/ToEnglishTranslationVisitorTest.osm
INPUT_DIR_2=test-files/cmd/glacial/RndToEnglishTranslationVisitorConvertCmdTest
GOLD_FILE=$INPUT_DIR_2/RndToEnglishTranslationVisitorConvertCmdTest-out.osm
OUTPUT_DIR=test-output/cmd/glacial/RndToEnglishTranslationVisitorConvertCmdTest
OUTPUT_FILE=$OUTPUT_DIR/RndToEnglishTranslationVisitorConvertCmdTest-out.osm

mkdir -p $OUTPUT_DIR

hoot convert -D convert.ops="hoot::ToEnglishTranslationVisitor" -D language.translation.translator=hoot::HootServicesTranslatorMockClient -D language.info.provider=hoot::HootServicesTranslationInfoMockClient -D language.translation.source.languages="de;es" -D language.tag.keys="name;alt_name" -D language.translation.perform.exhaustive.search.with.no.detection=false -D language.translation.detected.language.overrides.specified.source.languages=false $INPUT_FILE $OUTPUT_FILE

hoot diff $GOLD_FILE $OUTPUT_FILE
