#!/bin/bash
set -e

# This tests replacement changeset generation where the reference map includes feature types not found in the secondary map. In that respect it
# is similar to ServiceChangesetReplacementSecFilteredToEmptyTest, except the feature types involved are different.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementDifferingTypesTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementDifferingTypesTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementDifferingTypesTest/input2.osm" "11.361053,8.507705,11.363033,8.509146" "-180,-90,180,90" "" "" "false" "" "" "false" "" "5.0" "5.0"
