#!/bin/bash
set -e

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementAdditionalFiltersTest" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm" "test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm" "-122.43204,37.7628,-122.4303457,37.76437" "false" "hoot::PoiCriterion" "hoot::TagCriterion;hoot::TagValueNumericRangeCriterion" "false" "-D tag.criterion.kvps='name=Bombay' -D tag.value.numeric.range.criterion.keys=addr:housenumber -D tag.value.numeric.range.criterion.min=2217 -D tag.value.numeric.range.criterion.max=2217" "xml"
