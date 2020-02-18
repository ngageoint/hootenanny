#!/bin/bash
set -e

# This tests replacement changeset generation with TODO

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementDifferingTypesTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementDifferingTypesTest/input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementDifferingTypesTest/input2.osm" "29.031372,1.345701,29.036865,1.351193" "-180,-90,180,90" "true" "false" "" "" "false" "" "" "false" "" "xml" "5.0" "5.0" "false"
