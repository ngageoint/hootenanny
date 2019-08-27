#!/bin/bash
set -e

# replace schools and POIs with "School" in the name in the reference dataset with schools and POIs with "LIBRARY" in the name from the secondary dataset (OR the two filter criterion together for each input)
test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementInputFiltersTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "true" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagContainsCriterion" "false" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=School'" "hoot::TagCriterion;hoot::TagContainsCriterion" "false" "tag.criterion.kvps='amenity=school';tag.contains.criterion.kvps='name=LIBRARY'" "xml"
