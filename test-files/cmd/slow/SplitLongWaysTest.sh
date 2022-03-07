#!/bin/bash
set -e

export HOOT_OPTS="--warn -C Testing.conf"
export ADDITIONAL_VISITORS="SchemaTranslationVisitor;SplitLongWaysVisitor"
export TRANSLATION="TDSv40.js"
export OUTPUT_DIR=$HOOT_HOME/test-output/cmd/slow/SplitLongWaysTest
export DATA_DIR=$HOOT_HOME/test-files/cmd/slow/SplitLongWaysTest
export INPUTS=$DATA_DIR/SplitLongWaysTest.shp
export KNOWN_GOOD_OUTPUT=$DATA_DIR/SplitLongWaysTestResults.osm.pbf
export TEST_OUTPUT=$OUTPUT_DIR/SplitLongWaysTest.osm #.pbf

# Wipe out output dir so we don't get stale data and false pass
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot convert $HOOT_OPTS -D duplicate.node.remover.distance.threshold=1.0 -D schema.translation.script=$HOOT_HOME/translations/$TRANSLATION -D convert.ops+=$ADDITIONAL_VISITORS $INPUTS $TEST_OUTPUT

hoot diff $HOOT_OPTS $KNOWN_GOOD_OUTPUT $TEST_OUTPUT && echo "Test Complete"
