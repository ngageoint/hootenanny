#!/bin/bash
set -e

#export INPUTS="test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm"
export ADDITIONAL_VISITORS="hoot::SplitLongLinearWaysVisitor"
export TRANSLATION="TDSv40.js"
export OUTPUT_DIR=$HOOT_HOME/test-output/cmd/SplitLongWaysTest
export INPUTS=test-files/SplitLongWaysTest/
export KNOWN_GOOD_OUTPUT=$HOOT_HOME/test-files/SplitLongWaysTest/KnownGoodOutput.osm

# Wipe out output dir so we don't get stale data and false pass
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot ogr2osm -D ogr2osm.ops+=$ADDITIONAL_VISITORS $HOOT_HOME/translations/$TRANSLATION \
    $OUTPUT_DIR/LongWaysTest.osm $INPUTS

# Remove all lines with ingest timestamp from output as they'll differ every run and blow up the diff
sed -i '/source:ingest:datetime/c\' $OUTPUT_DIR/LongWaysTest.osm 
    
diff --brief $OUTPUT_DIR/LongWaysTest.osm $KNOWN_GOOD_OUTPUT
