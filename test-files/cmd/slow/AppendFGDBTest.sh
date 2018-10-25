#!/bin/bash
set -e

HOOT_OPT="--error"

INPUT_OSM=test-files/OsmMapAppendTest.osm

# We are making a FGDB. we need a clean directory
OUTPUT_DIR=test-output/cmd/slow/AppendFGDBTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

# Make an initial FGDB
hoot convert $HOOT_OPT $INPUT_OSM $OUTPUT_DIR/mgcp.gdb --trans translations/MGCP_TRD4.js

START_NUM=`ogrinfo -ro -al -so $OUTPUT_DIR/mgcp.gdb | grep "Feature Count" | awk '{print $3}'`

# Now add more features to the FGDB
hoot convert $HOOT_OPT -D ogr.append.data=true $INPUT_OSM $OUTPUT_DIR/mgcp.gdb --trans translations/MGCP_TRD4.js

END_NUM=`ogrinfo -ro -al -so $OUTPUT_DIR/mgcp.gdb | grep "Feature Count" | awk '{print $3}'`

# Make sure that the we have double the number of features at the end
if [ $END_NUM -ne $(($START_NUM*2)) ]; then
  echo "Failed: Start = $START_NUM, End = $END_NUM"
fi

