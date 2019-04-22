#!/bin/bash
set -e

rm -rf test-output/cmd/slow/ConvertCmdTest
mkdir -p test-output/cmd/slow/ConvertCmdTest

echo "Multiple OSM to single OSM..."
hoot convert -C Testing.conf test-files/ToyTestA.osm test-files/ToyTestB.osm test-output/cmd/slow/ConvertCmdTest/ToyTestC.osm
hoot diff test-files/cmd/slow/ConvertCmdTest/ToyTestC.osm test-output/cmd/slow/ConvertCmdTest/ToyTestC.osm

echo "PBF to OSM..."
hoot convert -C Testing.conf test-files/ToyTestA.osm.pbf test-output/cmd/slow/ConvertCmdTest/ToyTestA.osm
cat test-output/cmd/slow/ConvertCmdTest/ToyTestA.osm

echo "PBF to SHP..."
hoot convert -C Testing.conf test-files/ToyTestA.osm.pbf test-output/cmd/slow/ConvertCmdTest/ToyTestA.shp

echo "SHP to OSM..."
# comparing shapefiles is kinda hard. We'll convert it to a .osm file first.
hoot convert -C Testing.conf test-output/cmd/slow/ConvertCmdTest/ToyTestA.shp test-output/cmd/slow/ConvertCmdTest/ToyTestAShp.osm
hoot diff test-output/cmd/slow/ConvertCmdTest/ToyTestAShp.osm test-files/cmd/slow/ConvertCmdTest/ToyTestAShp.osm

echo "Streaming XML read and write..."
hoot convert -C Testing.conf -D writer.xml.sort.by.id="false" test-files/ToyTestA.osm test-output/cmd/slow/ConvertCmdTest/ToyTestA2.osm
hoot diff test-files/ToyTestA.osm test-output/cmd/slow/ConvertCmdTest/ToyTestA2.osm

echo "POI Criterion..."
# test streaming filter output to pois only
hoot convert -C Testing.conf -D writer.xml.sort.by.id="false" -D convert.ops="hoot::PoiCriterion" test-files/conflate/unified/AllDataTypesA.osm.pbf test-output/cmd/slow/ConvertCmdTest/AllDataTypesA.osm
hoot diff test-files/cmd/slow/ConvertCmdTest/AllDataTypesAPois.osm test-output/cmd/slow/ConvertCmdTest/AllDataTypesA.osm

echo "Check for invalid characters. You should see 3 sets of warnings."
hoot convert -C Testing.conf test-files/io/InvalidCharacters.osm test-output/cmd/slow/ConvertCmdTest/InvalidCharacters.osm
