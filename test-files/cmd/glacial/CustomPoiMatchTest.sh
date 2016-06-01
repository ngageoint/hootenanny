#!/bin/bash
set -e

mkdir -p test-output/cmd/CustomPoiMatchTest
hoot --unify -D merger.creators=hoot::CustomPoiMergerCreator -D match.creators=hoot::CustomPoiMatchCreator test-files/conflate/point/Poi1.osm test-files/conflate/point/Poi2.osm test-output/cmd/CustomPoiMatchTest/Conflated.osm
hoot --is-match test-output/cmd/CustomPoiMatchTest/Conflated.osm test-files/cmd/glacial/CustomPoiMatchTest.osm || diff test-output/cmd/CustomPoiMatchTest/Conflated.osm test-files/cmd/glacial/CustomPoiMatchTest.osm

