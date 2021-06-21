#!/bin/bash
set -e

IN_DIR_1=test-files/cmd/slow/AlphaShapeCmdTest
IN_DIR_2=test-files/cmd/slow/CutCmdTest
OUT_DIR=test-output/cmd/slow/CutCmdTest
mkdir -p $OUT_DIR

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

echo ""
echo "Cutting shape out..."
echo ""
hoot cut $LOG_LEVEL $CONFIG $IN_DIR_1/output-1.osm test-files/DcTigerRoads.osm $OUT_DIR/output-1.osm
hoot diff $LOG_LEVEL $CONFIG $OUT_DIR/output-1.osm $IN_DIR_2/output-1.osm || diff $OUT_DIR/output-1.osm $IN_DIR_2/output-1.osm

echo ""
echo "Cutting shape out with crop option..."
echo ""
hoot cut $LOG_LEVEL $CONFIG $IN_DIR_1/output-1.osm test-files/DcTigerRoads.osm $OUT_DIR/output-2.osm --crop
hoot diff $LOG_LEVEL $CONFIG $OUT_DIR/output-2.osm $IN_DIR_2/output-2.osm || diff $OUT_DIR/output-2.osm $IN_DIR_2/output-2.osm

echo ""
echo "Cutting shape out with alpha value..."
echo ""
hoot cut $LOG_LEVEL $CONFIG $IN_DIR_1/output-1.osm test-files/DcTigerRoads.osm $OUT_DIR/output-3.osm -100
hoot diff $LOG_LEVEL $CONFIG $OUT_DIR/output-3.osm $IN_DIR_2/output-3.osm || diff $OUT_DIR/output-3.osm $IN_DIR_2/output-3.osm

echo ""
echo "Cutting shape out with crop option and alpha value..."
echo ""
hoot cut $LOG_LEVEL $CONFIG $IN_DIR_1/output-1.osm test-files/DcTigerRoads.osm $OUT_DIR/output-4.osm -100 --crop
hoot diff $LOG_LEVEL $CONFIG $OUT_DIR/output-4.osm $IN_DIR_2/output-4.osm || diff $OUT_DIR/output-4.osm $IN_DIR_2/output-4.osm
