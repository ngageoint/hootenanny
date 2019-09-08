#!/bin/bash
set -e

# This tests replacement changeset generation when filters are imposed on the data being used to replace that are not chained together.
#
# Replace schools and POIs with "Library" in the name in the reference dataset with schools and POIs having "LIBRARY" in the name from the 
# secondary dataset (no chaining for filters: OR the replacement filter criteria together; OR the retainment filter criteria together). 
#
# - Should see "Sanchez Elementary School" in the reference data replaced by "Sanchez Elementary School" and "Sanchez Preschool" from the 
#   secondary data (not conflated), since "Sanchez Elementary School" in the ref data overlaps with that school in the sec data. 
# - Should see "Eureka Valley-Harvey Milk Memorial Branch Library" in the reference data be conflated with "EUREKA VALLEY BRANCH LIBRARY",
#   since the two overlap with each other.
# - Nothing else in the reference dataset should be modified or removed. Nothing else from the secondary dataset should have been added.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementAdditionalFiltersTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "-122.43384,37.76069,-122.42742,37.76869" "false" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "false" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=LIBRARY'" "hoot::TagCriterion;hoot::TagContainsCriterion" "false" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=School'" "xml"
