#!/bin/bash
set -e

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementMultipleGeometryFiltersStrictTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "true" "hoot::BuildingCriterion;hoot::HighwayCriterion" "" "false" "" "xml" "45.0" "45.0"
