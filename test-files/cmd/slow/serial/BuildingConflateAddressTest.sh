#!/bin/bash
set -e

# See related note in PoiPolygonConflateAddressTest.

OUT_DIR=test-output/cmd/glacial/BuildingConflateAddressTest

mkdir -p $HOOT_HOME/tmp/
mkdir -p $OUT_DIR
CONFIG="-C ReferenceConflation.conf -C UnifyingAlgorithm.conf -C Testing.conf -D uuid.helper.repeatable=true -D match.creators=hoot::BuildingMatchCreator -D merger.creators=hoot::BuildingMergerCreator -D address.match.enabled=true -D address.scorer.enable.caching=true"
LOG_LEVEL="--warn"

echo ""
echo "building-3441-addresses-1"
echo ""
IN_DIR=test-files/cases/reference/unifying/building/building-3441-addresses-1.off
hoot conflate $LOG_LEVEL $CONFIG $IN_DIR/Input1.osm $IN_DIR/Input2.osm \
  $OUT_DIR/building-3441-addresses-1-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/Expected.osm $OUT_DIR/building-3441-addresses-1-out.osm

echo ""
echo "building-3441-addresses-2"
echo ""
IN_DIR=test-files/cases/reference/unifying/building/building-3441-addresses-2.off
hoot conflate $LOG_LEVEL $CONFIG $IN_DIR/Input1.osm $IN_DIR/Input2.osm \
  $OUT_DIR/building-3441-addresses-2-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/Expected.osm $OUT_DIR/building-3441-addresses-2-out.osm
