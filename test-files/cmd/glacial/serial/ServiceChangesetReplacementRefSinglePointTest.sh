#!/bin/bash
set -e

# This tests that alpha shape generation doesn't fail if the ref map gets filtered down to a single POI.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRefSinglePointTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRefSinglePointTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRefSinglePointTest/input2.osm" "52.938359,32.727214,53.769359,32.853214" "-180,-90,180,90" "true" "true" "" "" "false" "" "" "" "" "xml" "5.0" "5.0" "true"
