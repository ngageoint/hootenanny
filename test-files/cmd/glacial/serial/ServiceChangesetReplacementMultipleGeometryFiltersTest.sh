#!/bin/bash
set -e

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementMultipleGeometryFiltersTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "false" "false" "hoot::BuildingCriterion;hoot::HighwayCriterion" "" "false" "" "xml" "20.0" "20.0"
