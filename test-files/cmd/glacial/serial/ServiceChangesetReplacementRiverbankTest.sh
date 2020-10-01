#!/bin/bash
set -e

# This tests replacement of water features.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRiverbankTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRiverbankTest/Input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRiverbankTest/Input2.osm" "-180,-90,180,90" "-180,-90,180,90" "true" "lenient" "" "" "false" "" "" "false" "" "xml" "5.0" "0.5" "false" "true"
