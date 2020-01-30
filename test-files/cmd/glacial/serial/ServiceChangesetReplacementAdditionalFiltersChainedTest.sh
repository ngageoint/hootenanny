#!/bin/bash
set -e

# This tests replacement changeset generation when filters are imposed on the data being used to replace that are chained together.
#
# Replace all schools with "School" in the name in the reference data with schools having "Preschool" in the name from the secondary dataset 
# (chaining enabled for filters: AND the replacement filter criteria together; AND the retainment filter criteria together).
#
# - Should see "Sanchez Elementary School" in the reference data conflated with "Sanchez Preschool" in the secondary data, since "Sanchez 
#   Elementary School" in the ref data overlaps with that school in the sec data. 
# - Nothing else in the reference dataset should be modified or removed. Nothing else from the secondary dataset should have been added.
#
# TODO: change all of these scripts to assign a variable name for each input beforehand to make this easier to read

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementAdditionalFiltersChainedTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43208,37.76075,-122.42892,37.7647" "-122.43384,37.76069,-122.42742,37.76869" "false" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "true" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=Preschool'" "hoot::TagCriterion;hoot::TagContainsCriterion" "true" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=School'" "xml"
