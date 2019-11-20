#!/bin/bash
set -e

# This tests replacement changeset generation with inputs that result in an empty secondary map after filtering when generating the points 
# changeset.

test-files/cmd/glacial/serial/RndServiceChangesetReplacement.sh.off "RndServiceChangesetReplacementSecFilteredToEmptyTest" "test-files/cmd/glacial/serial/RndServiceChangesetReplacementSecFilteredToEmptyTest/input1.osm" "test-files/cmd/glacial/serial/RndServiceChangesetReplacementSecFilteredToEmptyTest/input2.osm" "29.031372,1.345701,29.036865,1.351193" "-180,-90,180,90" "true" "false" "" "" "false" "" "" "" "" "xml" "5.0" "5.0"
