#!/bin/bash
set -e

# This test ensures that features belonging to relations that are not part of the OSM spec are not dropped.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementOutOfSpecMixedRelationsTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input2.osm" "-115.184767,36.031262,-115.048556,36.14796" "-180,-90,180,90" "true" "true" "" "" "false" "" "" "false" "" "xml" "5.0" "0.5" "true"
