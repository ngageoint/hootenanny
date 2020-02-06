#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/CountCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/conflate/unified/AllDataTypesA.osm
INPUT_FILE_2=test-files/conflate/unified/AllDataTypesB.osm
INPUT_FILE_3=test-files/conflate/generic/rivers/Haiti_CNIGS_Rivers_REF1-cropped-2.osm
INPUT_FILE_4=test-files/conflate/generic/rivers/Haiti_osm_waterway_ss_REF2-cropped-2.osm

echo "counting all features..."
hoot count --warn "$INPUT_FILE_1;$INPUT_FILE_2"

echo "counting all elements..."
hoot count --warn "$INPUT_FILE_1;$INPUT_FILE_2" --all-elements

# LinearWaterwayCriterion is not a map consumer, so streaming I/O can occur.

echo "counting all rivers..."
hoot count --warn "$INPUT_FILE_3;$INPUT_FILE_4" hoot::LinearWaterwayCriterion

echo "counting all elements that are not rivers..."
hoot count --warn -D element.criterion.negate=true "$INPUT_FILE_3;$INPUT_FILE_4" hoot::LinearWaterwayCriterion

# PoiCriterion is a map consumer, so streaming I/O cannot occur.

echo "counting all POIs..."
hoot count --warn "$INPUT_FILE_1;$INPUT_FILE_2" hoot::PoiCriterion

echo "counting all elements that are not POIs..."
hoot count --warn -D element.criterion.negate=true "$INPUT_FILE_1;$INPUT_FILE_2" hoot::PoiCriterion
