#!/bin/bash
set -e

# This tests replacement changeset generation with inputs that result in an empty ref map after filtering when generating the points changeset.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRefFilteredToEmptyTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRefFilteredToEmptyTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRefFilteredToEmptyTest/input2.osm" "-77.114735,38.884001,-77.112229,38.885158" "-180,-90,180,90" "" "" "false" "" "" "" "" "xml" "5.0" "5.0"
