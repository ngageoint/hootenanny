#!/bin/bash
set -e

# This tests replacement changeset generation with overlapping buildings being replaced with a lenient interpretation of the AOI boundaries.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementBuildingTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "-71.4809,42.4808,-71.45688,42.49368" "false" "false" "hoot::BuildingCriterion" "" "false" "" "" "" "" "db;xml;json" "5.0" "0.5" "false" "false" "true"
