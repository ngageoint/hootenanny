#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/NonContiguousRoadReviewsTest

hoot conflate -C Network.conf --warn test-files/Congo_MGCP_Roads_Bridges_subset.osm test-files/Congo_OSM_Roads_Bridges_subset.osm test-output/cmd/NonContiguousRoadReviewsTest/output.osm
hoot is-match test-output/cmd/NonContiguousRoadReviewsTest/output.osm test-files/cmd/glacial/NonContiguousRoadReviewsTest/output.osm || diff test-output/cmd/NonContiguousRoadReviewsTest/output.osm test-files/cmd/glacial/NonContiguousRoadReviewsTest/output.osm 
