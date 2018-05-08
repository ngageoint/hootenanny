#!/bin/bash

# This test is similar to ServiceOsmApiDbHootApiDbDcStreetsConflateTest, but is specifically checking to see
# that reference IDs are retained when the DualWaySplitter is used.
scripts/core/ServiceOsmApiDbHootApiDbConflate.sh \
  test-files/DcGisRoads.osm \
  test-files/DcTigerRoads.osm \
  -77.0535,38.8865,-77.0477,38.8913 unifying RndServiceOsmApiDbHootApiDbDcStreetsConflateTest2 glacial false -1
