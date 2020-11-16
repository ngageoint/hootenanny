#!/bin/bash
set -e

# This tests a cut with no replace. It must always run with the strict bounds interpretation to avoid unnecessary deletion of data.
#
# Note the highway=primary running through the middle of the cut bounds. It becomes part of a multilinestring relation due to the cropping. Not
# sure if that is how we want the output.
#
# TODO: move this to ServiceChangesetReplacementGridTest

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementDeleteOnlyTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementSecFilteredToEmptyTest/input1.osm" "test-files/Empty.osm" "29.031372,1.345701,29.036865,1.351193" "-180,-90,180,90" "" "" "false" "" "" "" "" "45.0" "45.0"
