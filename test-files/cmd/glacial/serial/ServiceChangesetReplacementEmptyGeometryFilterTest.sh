#!/bin/bash
set -e

# This tests replacement changeset generation with no geometry type filters, causing all feature types to be replaced.
#
# TODO: rework after #4267

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementEmptyGeometryFilterTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "-71.4809,42.4808,-71.45688,42.49368" "" "" "false" "" "" "" "" "20.0" "20.0"
