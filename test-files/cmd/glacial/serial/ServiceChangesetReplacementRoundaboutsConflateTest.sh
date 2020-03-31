#!/bin/bash
set -e

# Tests to see if we're handling roundabouts during conflate correctly in the replacement changeset workflow

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRoundaboutsConflateTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRoundaboutsConflateTest/Input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRoundaboutsConflateTest/Input2.osm" "-89.625447,20.99394,-89.612892,21.002564" "-180,-90,180,90" "true" "true" "" "" "false" "" "" "false" "" "xml" "5.0" "0.5" "false"
