#!/bin/bash
set -e

# This tests replacement changeset generation with overlapping POIs being replaced and a strict interpretation of the AOI boundaries.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementPoiStrictTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "-122.43384,37.76069,-122.42742,37.76869" "false" "true" "hoot::PoiCriterion" "" "false" "" "" "" "" "db;xml;json"
