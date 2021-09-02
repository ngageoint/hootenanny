#!/bin/bash
set -e

HOOT_OPTS="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf -C Testing.conf -D match.creators=HighwayMatchCreator;BuildingMatchCreator;ScriptMatchCreator,Poi.js -D merger.creators=HighwayMergerCreator;BuildingMergerCreator;ScriptMergerCreator -D uuid.helper.repeatable=true"

export INPUTS="test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm"
export OUTPUT_DIR=test-output/cmd/slow/ConflateConsistencyTest
mkdir -p $OUTPUT_DIR
hoot conflate $HOOT_OPTS $INPUTS $OUTPUT_DIR/Output1.osm
hoot conflate $HOOT_OPTS $INPUTS $OUTPUT_DIR/Output2.osm

diff --brief $OUTPUT_DIR/Output1.osm $OUTPUT_DIR/Output2.osm
