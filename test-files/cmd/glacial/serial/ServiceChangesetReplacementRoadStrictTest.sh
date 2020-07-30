#!/bin/bash
set -e

# This tests replacement changeset generation with overlapping roads being replaced and a strict interpretation of the AOI boundaries.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRoadStrictTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "-71.4809,42.4808,-71.45688,42.49368" "false" "strict" "hoot::HighwayCriterion" "" "false" "" "" "" "" "xml" "45.0" "45.0" "false" "false" "true"
