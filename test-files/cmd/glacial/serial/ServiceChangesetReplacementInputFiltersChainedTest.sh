#!/bin/bash
set -e

# replace POIs in the reference dataset that overlap with schools having "Preschool" in the name from the secondary dataset (AND the filter 
# criterion together)
test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementInputFiltersChainedTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "true" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=Preschool'" "xml"
