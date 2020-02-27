#!/bin/bash
set -e

# This tests replacement changeset generation with inputs that result in an empty secondary map after filtering when generating the points 
# changeset. Its not using full replacement, so the points in the reference dataset should not get deleted.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementSecFilteredToEmpty2Test" "test-files/cmd/glacial/serial/ServiceChangesetReplacementSecFilteredToEmptyTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementSecFilteredToEmptyTest/input2.osm" "29.031372,1.345701,29.036865,1.351193" "-180,-90,180,90" "false" "false" "" "" "false" "" "" "" "" "xml" "5.0" "5.0"
