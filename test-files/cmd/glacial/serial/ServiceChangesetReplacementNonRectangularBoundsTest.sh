#!/bin/bash
set -e

# This tests replacement changeset generation with a non-rectangular polygon bounds.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementNonRectangularBoundsTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4745,42.4841;-71.4669,42.4918;-71.4619,42.4839;-71.4745,42.4841" "-71.4809,42.4808,-71.45688,42.49368" "" "" "false" "" "" "" "" "true" "5.0" "0.5"
