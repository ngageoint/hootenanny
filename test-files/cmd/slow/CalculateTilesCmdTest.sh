#!/bin/bash
set -e

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep tiles-calculate`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "tiles-calculate command not present.  Exiting test without running it."
 exit 0
fi

OUTPUT_DIR=test-output/cmd/slow/CalculateTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/CalculateTilesCmdTest
mkdir -p $OUTPUT_DIR 

hoot tiles-calculate --warn "test-files/DcGisRoads.osm;test-files/DcTigerRoads.osm" $OUTPUT_DIR/output.geojson 1000 0.001
diff $GOLD_FILES_DIR/output.geojson $OUTPUT_DIR/output.geojson

