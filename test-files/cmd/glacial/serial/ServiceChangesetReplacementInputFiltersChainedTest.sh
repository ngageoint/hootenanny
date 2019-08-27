#!/bin/bash
set -e

# replace only schools with "Elementary" in the name in the reference dataset with only schools with "Elementary" in the name from the 
# secondary dataset (AND the two criterion together)
test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementInputFiltersChainedTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "true" "hoot::TagCriterion;hoot::TagContainsCriterion" "true" "-D tag.criterion.kvps='amenity=school' -D tag.contains.criterion.kvps='name=Elementary'" "xml"
