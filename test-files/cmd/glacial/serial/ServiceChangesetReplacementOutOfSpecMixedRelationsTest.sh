#!/bin/bash
set -e

# This test ensures that features belonging to relations that are not part of the OSM spec are not dropped. There also shouldn't be any 
# duplicated features in the output which can happen when features belonging to relations are separated by geometry during processing.

# This also tests handling of relations whose members are missing from the input. The secondary input file has one way relation member 
# commented out (unfortunately this makes it unreadable in JOSM, so you'll have to temporarily uncomment it to view the input files there). 
# Any relations in the output owning the commented out member should be tagged with the 'hoot::missing_child' tag after the changeset is 
# applied.
#
# The main one to check here is the large unlabeled green way that is inside the shop=mall way named "The Forum Shops at Caesars". That way
# may get unncessarily duplicated due to belonging to multiple relations. Drag it slightly to ensure a duplicate isn't hidden underneath of it.
# Also, make sure it is still connected to all the shop polys it should be.
#
# Output issues: 
# 1. There is a poorly snapped untyped way belonging to a multilinestring relation between "LVB Burger" and "Rhumber".

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementOutOfSpecMixedRelationsTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementOutOfSpecMixedRelationsTest/input2.osm" "-115.184767,36.031262,-115.048556,36.14796" "-180,-90,180,90" "" "" "false" "" "" "false" "" "5.0" "0.5"
