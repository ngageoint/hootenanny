#!/bin/bash

# input data prep
#hoot crop -D writer.include.debug.tags=true test-files/BostonSubsetRoadBuilding_FromOsm.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbBuildingConflateTest/BostonSubsetRoadBuilding_FromOsm-cropped.osm "-71.4770,42.4832,-71.4755,42.4851"
#hoot crop -D writer.include.debug.tags=true test-files/BostonSubsetRoadBuilding_FromShp.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbBuildingConflateTest/BostonSubsetRoadBuilding_FromShp-cropped.osm "-71.4770,42.4832,-71.4755,42.4851"

# These two inputs were chosen, b/c they're the only place found so far in our test data where when merging two buildings
# the secondary building ID is kept by default.  This test activates preserve.unknown1.element.id.when.modifying.features,
# which will force the ref ID to be preserved.

scripts/core/ServiceOsmApiDbHootApiDbConflate.sh \
  test-files/cmd/glacial/RndServiceOsmApiDbHootApiDbBuildingConflateTest/BostonSubsetRoadBuilding_FromShp-cropped-4.osm \
  test-files/cmd/glacial/RndServiceOsmApiDbHootApiDbBuildingConflateTest/BostonSubsetRoadBuilding_FromOsm-cropped-4.osm \
  -71.4771,42.4835,-71.4758,42.4841 unifying RndServiceOsmApiDbHootApiDbBuildingConflateTest glacial false -1
