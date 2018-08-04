#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/CountCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/conflate/unified/AllDataTypesA.osm
INPUT_FILE_2=test-files/conflate/unified/AllDataTypesB.osm

# count all features
hoot count --warn "$INPUT_FILE_1;$INPUT_FILE_2"

# count all elements
hoot count --warn "$INPUT_FILE_1;$INPUT_FILE_2" --all-elements

# count all POIs
hoot count --warn "$INPUT_FILE_1;$INPUT_FILE_2" hoot::PoiCriterion

# count all elements that are not POIs
hoot count --warn -D element.criterion.negate=true "$INPUT_FILE_1;$INPUT_FILE_2" hoot::PoiCriterion
