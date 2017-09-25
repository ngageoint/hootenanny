#!/bin/bash

# input data prep
#hoot crop-map -D writer.include.debug.tags=true test-files/cmd/slow/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbPoiPolygonConflateTest/PoiPolygon1.osm "-122.4244,37.7618,-122.4196,42.7651"
#hoot crop-map -D writer.include.debug.tags=true test-files/cmd/slow/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbPoiPolygonConflateTest/PoiPolygon2.osm "-122.4244,37.7618,-122.4196,42.7651"

# by default the PoiPolygonMerger behaves as we expect w/o unknown1 ID preservation changes, so this test is here to ensure
# that doesn't change
scripts/core/ServiceOsmApiDbHootApiDbConflate.sh test-files/cmd/glacial/ServiceOsmApiDbHootApiDbPoiPolygonConflateTest/PoiPolygon1.osm test-files/cmd/glacial/ServiceOsmApiDbHootApiDbPoiPolygonConflateTest/PoiPolygon2.osm -122.4230,37.7620,-122.4204,37.7647 unifying ServiceOsmApiDbHootApiDbPoiPolygonConflateTest glacial false -1
