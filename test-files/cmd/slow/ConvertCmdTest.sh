#!/bin/bash
set -e

rm -rf test-output/cmd/ConvertCmdTest
mkdir -p test-output/cmd/ConvertCmdTest

hoot convert test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.osm
cat test-output/cmd/ConvertCmdTest/ToyTestA.osm

hoot convert test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.shp
# comparing shapefiles is kinda hard. We'll convert it to a .osm file first.
hoot convert test-output/cmd/ConvertCmdTest/ToyTestA.shp test-output/cmd/ConvertCmdTest/ToyTestAShp.osm
hoot is-match test-output/cmd/ConvertCmdTest/ToyTestAShp.osm test-files/cmd/slow/ConvertCmdTest/ToyTestAShp.osm

# this tests streaming xml output
hoot convert -D writer.xml.sort.by.id=false test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA2.osm
hoot is-match test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA2.osm

echo Check for invalid characters. You should see 3 sets of warnings.
hoot convert  test-files/io/InvalidCharacters.osm test-output/cmd/ConvertCmdTest/InvalidCharacters.osm


