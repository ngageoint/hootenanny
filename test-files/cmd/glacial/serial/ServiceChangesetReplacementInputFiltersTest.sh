#!/bin/bash
set -e

# replace POIs in the reference dataset that overlap with schools and POIs having "LIBRARY" in the name from the secondary dataset (OR the filter criterion together)
test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementInputFiltersTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "false" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=LIBRARY'" "xml"
