#!/bin/bash
set -e

# Replace POIs in the reference dataset that overlap with schools having "Preschool" in the name from the secondary dataset (AND the filter 
# criterion together).
#
# - Should see "Sanchez Elementary School" in the reference data completely replaced by "Sanchez Preschool" in the secondary data (not 
# conflated), since "Sanchez Elementary School" in the ref data overlaps with that school in the sec data. 
# - Nothing else in the reference dataset should be modified or removed. Nothing else from the secondary dataset should have been added.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementInputFiltersChainedTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "true" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=Preschool'" "xml"
