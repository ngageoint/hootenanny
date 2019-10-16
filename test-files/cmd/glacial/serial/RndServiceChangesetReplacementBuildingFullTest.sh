#!/bin/bash
set -e

# This tests replacement changeset generation with buildings being replaced throughout the entire AOI.

test-files/cmd/glacial/serial/RndServiceChangesetReplacement.sh.off "RndServiceChangesetReplacementBuildingFullTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "-71.4809,42.4808,-71.45688,42.49368" "true" "false" "hoot::BuildingCriterion" "" "false" "" "" "" "" "xml"
