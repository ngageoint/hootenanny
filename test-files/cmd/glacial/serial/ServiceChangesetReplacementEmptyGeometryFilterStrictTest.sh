#!/bin/bash
set -e

# "-71.47326,42.47609,-71.47255,42.47666"
test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementEmptyGeometryFilterStrictTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "true" "" "" "false" "" "xml" "45.0" "45.0"
