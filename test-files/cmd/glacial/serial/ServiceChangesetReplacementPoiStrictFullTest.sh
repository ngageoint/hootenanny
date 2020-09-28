#!/bin/bash
set -e

# This tests replacement changeset generation with POIs being replaced over the entire AOI and a strict interpretation of the AOI boundaries.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementPoiStrictFullTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "-122.43384,37.76069,-122.42742,37.76869" "true" "strict" "hoot::PoiCriterion" "" "false" "" "" "" "" "xml" "5.0" "0.5" "false" "true"
