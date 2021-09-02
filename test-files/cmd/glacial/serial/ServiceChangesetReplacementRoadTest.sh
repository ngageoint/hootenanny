#!/bin/bash
set -e

# This tests replacement changeset generation with roads being replaced over the entire AOI and a lenient interpretation of 
# the AOI boundaries.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRoadTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "-71.4809,42.4808,-71.45688,42.49368" "HighwayCriterion;RelationWithHighwayMembersCriterion" "true" "20.0" "20.0"
