#!/bin/bash
set -e

# This tests replacement changeset generation with multiple geometry type filters, causing multiple feature types to be replaced.
#
# TODO: rework after #4267

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementMultipleGeometryFiltersTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "-71.4809,42.4808,-71.45688,42.49368" "hoot::BuildingCriterion;hoot::HighwayCriterion" "" "false" "" "" "" "" "20.0" "20.0"
