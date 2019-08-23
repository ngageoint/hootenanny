#!/bin/bash
set -e

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off ServiceChangesetReplacementAdditionalFilterTest test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm -122.43204,37.7628,-122.4303457,37.76437 false hoot::PoiCriterion "hoot::TagContainsCriterion;hoot::TagCriterion" true "-D tag.criterion.kvps='amenity=cafe' -D tag.contains.criterion.kvps='name=Java'" "xml"
