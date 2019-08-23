#!/bin/bash

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off ServiceChangesetReplacementPoiStrictTest test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon1.osm test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/PoiPolygon2.osm -122.43204,37.7628,-122.4303457,37.76437 true hoot::PoiCriterion "" "db;xml;json"
