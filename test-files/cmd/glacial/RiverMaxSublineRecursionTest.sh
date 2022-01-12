#!/bin/bash
set -e

# This tests that River Conflation properly switches subline matchers when a river feature is
# encountered that pushes subline processing over the subline recursion threshold 
# (maximal.subline.max.recursions). We're not even checking output here and just checking that the
# conflate job does not crash, which can happen if RecursiveComplexityException is improperly 
# handled in River.js. The subline recursion threshold for rivers is calculated in 
# RiverMaximalSublineSettingOptimizer. If changes are made to RiverMaximalSublineSettingOptimizer
# that increase the subline recursion threshold, then new source data may have to be acquired for
# this test.

LOG_LEVEL="--warn"
TEST_NAME=RiverMaxSublineRecursionTest
IN_DIR=test-files/cmd/glacial/$TEST_NAME
OUT_DIR=test-output/cmd/glacial/$TEST_NAME
mkdir -p $OUT_DIR

# Its very expensive to only remove partial matches for this dataset, so we disable that feature.
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 -D differential.remove.linear.partial.matches.as.whole=true \
 -D differential.remove.river.partial.matches.as.whole=true \
 -D match.creators=ScriptMatchCreator,River.js \
 -D merger.creators=ScriptMergerCreator \
 -D differential.snap.unconnected.features=true \
 -D snap.unconnected.ways.snap.criteria=HighwayCriterion \
 -D conflate.differential.include.tags=true \
 -D conflate.differential.tags.separate.output=true \
 $IN_DIR/Input1.osm $IN_DIR/Input2.osm $OUT_DIR/diff.osc --changeset-stats $OUT_DIR/stats.json
