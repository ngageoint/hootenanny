#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/CountCmdTest
OUTPUT_DIR=test-output/cmd/slow/CountCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
INPUT_FILE_1=test-files/conflate/unified/AllDataTypesA.osm
INPUT_FILE_2=test-files/conflate/unified/AllDataTypesB.osm
INPUT_FILE_3=test-files/conflate/generic/rivers/Haiti_CNIGS_Rivers_REF1-cropped-2.osm
INPUT_FILE_4=test-files/conflate/generic/rivers/Haiti_osm_waterway_ss_REF2-cropped-2.osm
INPUT_FILE_5=test-files/cmd/slow/CountCmdTest/inputDir

CONFIG="-C Testing.conf"

echo ""
echo "counting all features..."
hoot count --warn $CONFIG "$INPUT_FILE_1;$INPUT_FILE_2"

echo ""
echo "counting all elements..."
hoot count --warn $CONFIG "$INPUT_FILE_1;$INPUT_FILE_2" --all-elements

# RiverCriterion is not a map consumer, so streaming I/O will occur.

echo ""
echo "counting all rivers..."
hoot count --warn $CONFIG "$INPUT_FILE_3;$INPUT_FILE_4" RiverCriterion

echo ""
echo "counting all elements that are not rivers..."
hoot count --warn $CONFIG -D element.criteria.negate=true "$INPUT_FILE_3;$INPUT_FILE_4" RiverCriterion

# PoiCriterion is a map consumer, so streaming I/O cannot occur.

echo ""
echo "counting all POIs..."
hoot count --warn $CONFIG "$INPUT_FILE_1;$INPUT_FILE_2" hoot::PoiCriterion

echo ""
echo "counting all elements that are not POIs..."
hoot count --warn $CONFIG -D element.criteria.negate=true "$INPUT_FILE_1;$INPUT_FILE_2" PoiCriterion

# Check to make sure multi-layer gdb's get parsed correctly.

echo ""
echo "counting all element from multi-layer GDB..."
hoot count $INPUT_DIR/input.gdb --all-elements

# Check combos of crits and crit related options

echo ""
echo "counting all rivers and POIs..."
hoot count --warn $CONFIG "$INPUT_FILE_1;$INPUT_FILE_2;$INPUT_FILE_3;$INPUT_FILE_4" "RiverCriterion;PoiCriterion"

echo ""
echo "counting elements that are both rivers and POIs..."
hoot count --warn $CONFIG -D element.criteria.chain=true "$INPUT_FILE_1;$INPUT_FILE_2;$INPUT_FILE_3;$INPUT_FILE_4" "RiverCriterion;PoiCriterion"

echo ""
echo "counting elements that are not POIs..."
hoot count --warn $CONFIG -D element.criteria.negate=true "$INPUT_FILE_1;$INPUT_FILE_2;$INPUT_FILE_3;$INPUT_FILE_4" PoiCriterion

echo ""
echo "counting recursively in a directory structure..."
hoot count --warn $CONFIG "$INPUT_FILE_1;$INPUT_FILE_5" --recursive

echo ""
echo "counting recursively in a directory structure with a criterion..."
hoot count --warn $CONFIG "$INPUT_FILE_1;$INPUT_FILE_5" HighwayCriterion --recursive
