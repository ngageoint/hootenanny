#!/bin/bash
set -e

# This test ensures that features belonging to relations that are not part of the OSM spec are not dropped. There also shouldn't be any 
# duplicated features in the output which can happen when features belonging to relations are separated by geometry during processing.

# This also tests handling of relations whose members are missing from the input. The secondary input file has one way relation member 
# commented out (unfortunately this makes it unreadable in JOSM, so you'll have to temporarily uncomment it to view the input files there). 
# Any relations in the output owning the commented out member should be tagged with the 'hoot::missing_child' tag after the changeset is 
# applied.
#
# Output issues: 
# 1. Drag the top left or right corners of "Victoria's Secret' down a bit and see the duplicated info nodes with the tag, "Map of the 
# Shopping Centre". Generally, way nodes don't get info tag so not a critical problem.
# 2. There's an empty relation in the output after disabling removal of missing elements.
#
# TODO: update the missing child ref portion of the test to handle ways
# TODO: add a non-full replacement test for the missing child ref portion of the test

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementOutOfSpecMixedRelationsTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input2.osm" "-115.184767,36.031262,-115.048556,36.14796" "-180,-90,180,90" "true" "strict" "" "" "false" "" "" "false" "" "xml" "5.0" "0.5" "true" "false" "true"
