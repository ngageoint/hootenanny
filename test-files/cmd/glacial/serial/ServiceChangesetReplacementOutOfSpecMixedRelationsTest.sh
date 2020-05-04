#!/bin/bash
set -e

# This test ensures that features belonging to relations that are not part of the OSM spec are not dropped.
# 
# Note: Note that one polygon in the test output, one with no name and no type but belonging to a type=level (out of spec) and type=multipolygon
# relation is duplicted in the output with one of the features disconnected. This will be dealt with in #3998.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementOutOfSpecMixedRelationsTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input2.osm" "-115.184767,36.031262,-115.048556,36.14796" "-180,-90,180,90" "true" "true" "" "" "false" "" "" "false" "" "xml" "5.0" "0.5" "true"
