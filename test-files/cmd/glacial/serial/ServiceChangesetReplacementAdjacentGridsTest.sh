#!/bin/bash
set -e

# TODO

OUT_DIR=test-output/cmd/glacial/serial/ServiceChangesetReplacementAdjacentGridsTest

# northern task 53
#test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementAdjacentGridsTest" "/home/vagrant/hoot/tmp/4158/NorthernTask53/NOME_849248_OSM_849248/NOME_849248.osm" "/home/vagrant/hoot/tmp/4158/NorthernTask53/NOME_849248_OSM_849248/OSM_849248.osm" "-115.048828,36.279707,-115.004883,36.315127" "-180,-90,180,90" "true" "false" "" "" "false" "" "" "" "" "xml" "20.0" "20.0" "true" "true"
# save off old files
#if test -f "$OUT_DIR/ref-original.osm"; then
#  cp $OUT_DIR/ref-original.osm $OUT_DIR/ref-original-north.osm
#fi
#if test -f "$OUT_DIR/sec-original.osm"; then
#  cp $OUT_DIR/sec-original.osm $OUT_DIR/sec-original-north.osm
#fi
#if test -f "$OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-xml-replaced.osm"; then
#  cp $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-xml-replaced.osm $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-xml-replaced-#north.osm
#fi
#if test -f "$OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-bounds.osm"; then
#  cp $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-bounds.osm $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-bounds-north.osm
#fi

# southern task 52 - FAILS
test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementAdjacentGridsTest" "/home/vagrant/hoot/tmp/4158/SouthernTask52/task_52_inputdata/NOME_d5a6dc.osm" "/home/vagrant/hoot/tmp/4158/SouthernTask52/task_52_inputdata/OSM_d5a6dc.osm" "-115.048828,36.244273,-115.004883,36.279709" "-180,-90,180,90" "true" "false" "" "" "false" "" "" "" "" "xml" "5.0" "0.5" "true" "true"
# rename files for clarity
#if test -f "$OUT_DIR/ref-original.osm"; then
#  cp $OUT_DIR/ref-original.osm $OUT_DIR/ref-original-south.osm
#fi
#if test -f "$OUT_DIR/sec-original.osm"; then
#  cp $OUT_DIR/sec-original.osm $OUT_DIR/sec-original-south.osm
#fi
#if test -f "$OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-xml-replaced.osm"; then
#  cp $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-xml-replaced.osm $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-xml-replaced-south.osm
#fi
#if test -f "$OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-bounds.osm"; then
#  cp $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-bounds.osm $OUT_DIR/ServiceChangesetReplacementAdjacentGridsTest-bounds-south.osm
#fi