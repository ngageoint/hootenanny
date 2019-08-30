#!/bin/bash
set -e

# Replace POIs in the reference dataset that overlap with schools and POIs having "LIBRARY" in the name from the secondary dataset (OR the 
# filter criterion together). 
#
# - Should see "Sanchez Elementary School" in the reference data completely replaced by "Sanchez Elementary School" and "Sanchez Preschool" in 
# the secondary data (not conflated), since "Sanchez Elementary School" in the ref data overlaps with those two schools in the sec data. 
# - Should see "Eureka Valley-Harvey Milk Memorial Branch Library" in the reference data be conflated with "EUREKA VALLEY BRANCH LIBRARY" since
# the ref feature lies just outside of the the secondary dataset alpha shape.
# - Nothing else in the reference dataset should be modified or removed. Nothing else from the secondary dataset should have been added.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementInputFiltersTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "false" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "false" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=LIBRARY'" "xml"
