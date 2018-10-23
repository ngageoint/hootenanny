#!/bin/bash
set -e

# See https://github.com/ngageoint/hootenanny/issues/586

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/NonContiguousRoadReviewsTest

HOOT_OPTS="-D poi.polygon.address.match.enabled=false"

hoot conflate --warn -C Network.conf $HOOT_OPTS test-files/Congo_MGCP_Roads_Bridges_subset.osm test-files/Congo_OSM_Roads_Bridges_subset.osm test-output/cmd/NonContiguousRoadReviewsTest/output.osm
hoot diff test-output/cmd/NonContiguousRoadReviewsTest/output.osm test-files/cmd/glacial/NonContiguousRoadReviewsTest/output.osm || diff test-output/cmd/NonContiguousRoadReviewsTest/output.osm test-files/cmd/glacial/NonContiguousRoadReviewsTest/output.osm 
