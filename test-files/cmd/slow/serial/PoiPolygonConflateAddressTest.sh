#!/bin/bash
set -e

# This replaces corresponding tests in the case tests since those can't be run in a serial fashion 
# and could result in unnecessary multiple calls to load the hefty libpostal data. Unfortunately, 
# this test will incur the lipostal load overhead for each command executed in it. Finishing #4908 
# would make this test obsolete.

OUT_DIR=test-output/cmd/glacial/PoiPolygonConflateAddressTest

mkdir -p $HOOT_HOME/tmp/
mkdir -p $OUT_DIR
CONFIG="-C ReferenceConflation.conf -C UnifyingAlgorithm.conf -C Testing.conf -D uuid.helper.repeatable=true -D match.creators=hoot::PoiPolygonMatchCreator -D merger.creators=hoot::PoiPolygonMergerCreator -D address.match.enabled=true -D address.scorer.enable.caching=true"
LOG_LEVEL="--warn"

echo ""
echo "poi-polygon-auto-merge-13"
echo ""
IN_DIR=test-files/cases/reference/unifying/poi-polygon/poi-polygon-auto-merge-13.off
hoot conflate $LOG_LEVEL $CONFIG $IN_DIR/Input1.osm $IN_DIR/Input2.osm \
  $OUT_DIR/poi-polygon-auto-merge-13-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/Expected.osm $OUT_DIR/poi-polygon-auto-merge-13-out.osm

echo ""
echo "poi-polygon-auto-merge-14"
echo ""
IN_DIR=test-files/cases/reference/unifying/poi-polygon/poi-polygon-auto-merge-14.off
hoot conflate $LOG_LEVEL $CONFIG -D poi.polygon.auto.merge.many.poi.to.one.poly.matches=true \
  $IN_DIR/Input1.osm $IN_DIR/Input2.osm $OUT_DIR/poi-polygon-auto-merge-14-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/Expected.osm $OUT_DIR/poi-polygon-auto-merge-14-out.osm

echo ""
echo "poi-polygon-10"
echo ""
IN_DIR=test-files/cases/reference/unifying/poi-polygon/poi-polygon-10.off
hoot conflate $LOG_LEVEL $CONFIG -D poi.polygon.match.evidence.threshold=2 \
  -D poi.polygon.promote.points.with.addresses.to.pois=true \
  $IN_DIR/Input1.osm $IN_DIR/Input2.osm $OUT_DIR/poi-polygon-10-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/Expected.osm $OUT_DIR/poi-polygon-10-out.osm

echo ""
echo "poi-polygon-recursive-way-address-3267-1"
echo ""
IN_DIR=test-files/cases/reference/unifying/poi-polygon/poi-polygon-recursive-way-address-3267-1.off
hoot conflate $LOG_LEVEL $CONFIG $IN_DIR/Input1.osm $IN_DIR/Input2.osm \
  $OUT_DIR/poi-polygon-recursive-way-address-3267-1-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/Expected.osm $OUT_DIR/poi-polygon-recursive-way-address-3267-1-out.osm

echo ""
echo "poi-polygon-review-conflict-4331-3"
echo ""
IN_DIR=test-files/cases/reference/unifying/poi-polygon/poi-polygon-review-conflict-4331-3.off
hoot conflate $LOG_LEVEL $CONFIG \
  -D match.creators="hoot::BuildingMatchCreator;hoot::PoiPolygonMatchCreator;hoot::ScriptMatchCreator,Poi.js" \
  -D merger.creators="hoot::BuildingMergerCreator;hoot::PoiPolygonMergerCreator;hoot::ScriptMergerCreator" \
  -D poi.polygon.name.string.comparer=hoot::KskipBigramDistance \
  $IN_DIR/Input1.osm $IN_DIR/Input2.osm $OUT_DIR/poi-polygon-review-conflict-4331-3-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/Expected.osm $OUT_DIR/poi-polygon-review-conflict-4331-3-out.osm
