#!/bin/bash
set -e

# This tests a cut with no replace. It must always run with the strict bounds interpretation to avoid unnecessary deletion of data.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementDeleteOnlyStrictFullTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementSecFilteredToEmptyTest/input1.osm" "test-files/Empty.osm" "29.031372,1.345701,29.036865,1.351193" "-180,-90,180,90" "true" "strict" "" "" "false" "" "" "" "" "xml" "45.0" "45.0" "false" "true"
