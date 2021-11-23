#!/bin/bash
set -e

# When matching only, secondary data and reviews should not be dropped from the Attribute Conflation
# output as they are during full conflation.

mkdir -p $HOOT_HOME/tmp/
IN_DIR=test-files/cmd/slow/AttributeConflateMatchOnlyTest
OUT_DIR=test-output/cmd/slow/AttributeConflateMatchOnlyTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf \
  -D conflate.use.data.source.ids.1="false" -D conflate.use.data.source.ids.2="true" \
  -D uuid.helper.repeatable=true -D writer.include.debug.tags=true \
  -D match.creators="HighwayMatchCreator" -D merger.creators="HighwayMergerCreator" \
  -D conflate.match.only=true test-files/ToyTestA.osm test-files/ToyTestB.osm $OUT_DIR/output.osm
hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
