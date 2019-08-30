#!/bin/bash
set -e

# "-71.47326,42.47609,-71.47255,42.47666"
test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementEmptyGeometryFilterTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.4698,42.4866,-71.4657,42.4902" "false" "false" "" "" "false" "" "xml" "20.0" "20.0"
