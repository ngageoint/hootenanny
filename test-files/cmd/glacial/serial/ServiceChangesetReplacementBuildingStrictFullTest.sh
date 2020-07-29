#!/bin/bash
set -e

# This tests replacement changeset generation with all buildings being replaced with a strict interpretation of the AOI boundaries.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementBuildingStrictFullTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "-71.4809,42.4808,-71.45688,42.49368" "true" "true" "hoot::BuildingCriterion" "" "false" "" "" "" "" "xml" "5.0" "0.5" "false" "false" "true"
