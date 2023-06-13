#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ConvertCmdTest
OUTPUT_DIR=test-output/cmd/slow/ConvertCmdTestDnc
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

TRANSLATION="-D schema.translation.script=translations"
TRANSLATION_DNC="$TRANSLATION/DNC.js"

echo ""
echo "DNC to OSM..."
echo ""
rm -rf $OUTPUT_DIR/DNC17
unzip -q $INPUT_DIR/coa17d.zip -d $OUTPUT_DIR/
hoot convert $LOG_LEVEL $CONFIG -D ogr.add.uuid=false -D reader.add.source.datetime=false $TRANSLATION_DNC \
  gltp:/vrf/$HOOT_HOME/$OUTPUT_DIR/DNC17/COA17D $OUTPUT_DIR/dnc_test.osm
# NOTE: The OGDI driver doesn't produce output in the exact same way every time so don't compare
# the outputs just ensure that no errors show up in the command output
#hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/dnc_test.osm $OUTPUT_DIR/dnc_test.osm

