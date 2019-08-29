#!/bin/bash
set -e

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementEmptyGeometryFilterTest" "test-files/BostonSubsetRoadBuilding_FromOsm.osm" "" "-71.47326,42.47609,-71.47255,42.47666" "false" "" "" "false" "" "xml" "20.0" "20.0"
