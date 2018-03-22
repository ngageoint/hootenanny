#!/bin/bash
set -e

rm -rf test-output/cmd/ConvertCmdTest
mkdir -p test-output/cmd/ConvertCmdTest

echo "PBF to OSM..."
hoot convert test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.osm
cat test-output/cmd/ConvertCmdTest/ToyTestA.osm

echo "PBF to SHP..."
hoot convert test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.shp
echo "SHP to OSM..."
# comparing shapefiles is kinda hard. We'll convert it to a .osm file first.
hoot convert test-output/cmd/ConvertCmdTest/ToyTestA.shp test-output/cmd/ConvertCmdTest/ToyTestAShp.osm
hoot map-diff test-output/cmd/ConvertCmdTest/ToyTestAShp.osm test-files/cmd/slow/ConvertCmdTest/ToyTestAShp.osm

echo "Streaming XML read and write..."
hoot convert -D writer.xml.sort.by.id="false" test-files/ToyTestA.osm test-output/cmd/ConvertCmdTest/ToyTestA2.osm
hoot map-diff test-files/ToyTestA.osm test-output/cmd/ConvertCmdTest/ToyTestA2.osm

echo "POI Criterion..."
# test streaming filter output to pois only
hoot convert -D writer.xml.sort.by.id="false" -D convert.ops="hoot::PoiCriterion" test-files/conflate/unified/AllDataTypesA.osm.pbf test-output/cmd/ConvertCmdTest/AllDataTypesA.osm
hoot map-diff test-files/cmd/slow/ConvertCmdTest/AllDataTypesAPois.osm test-output/cmd/ConvertCmdTest/AllDataTypesA.osm

echo "Check for invalid characters. You should see 3 sets of warnings."
hoot convert  test-files/io/InvalidCharacters.osm test-output/cmd/ConvertCmdTest/InvalidCharacters.osm
