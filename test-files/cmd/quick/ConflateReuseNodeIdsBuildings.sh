#!/bin/bash
set -e

IN_DIR=test-files/cmd/quick/ConflateReuseNodeIds
OUT_DIR=test-output/cmd/quick/ConflateReuseNodeIds

mkdir -p $OUT_DIR

HOOT_OPTS='--warn -D uuid.helper.repeatable=true -C Testing.conf'

hoot conflate $HOOT_OPTS \
 -C UnifyingAlgorithm.conf \
 -C ReferenceConflation.conf \
 -D match.creators="hoot::BuildingMatchCreator" \
 -D merger.creators="hoot::BuildingMergerCreator" \
 -D conflate.use.data.source.ids.2=true \
 $CONFLATE_OPTS $IN_DIR/BuildingsA.osm $IN_DIR/BuildingsB.osm $OUT_DIR/BuildingsConflated.osm

hoot diff $HOOT_OPTS $OUT_DIR/BuildingsConflated.osm $IN_DIR/BuildingsConflatedExpected.osm

hoot changeset-derive $HOOT_OPTS $IN_DIR/BuildingsA.osm $OUT_DIR/BuildingsConflated.osm $OUT_DIR/BuildingsConflated.osc
diff $OUT_DIR/BuildingsConflated.osc $IN_DIR/BuildingsConflatedExpected.osc

