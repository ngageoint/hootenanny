#!/bin/bash
set -e

rm -rf test-output/cmd/ConvertCmdTest
mkdir -p test-output/cmd/ConvertCmdTest
hoot --convert test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.osm
cat test-output/cmd/ConvertCmdTest/ToyTestA.osm

hoot --convert test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.shp
ls test-output/cmd/ConvertCmdTest/ToyTestA.shp

echo Check for invalid characters. You should see 3 sets of warnings.
hoot --convert  test-files/io/InvalidCharacters.osm test-output/cmd/ConvertCmdTest/InvalidCharacters.osm

hoot convert test-files/jakarta_raya_coastline.shp test-output/cmd/ConvertCmdTest/jakarta_raya_coastline.osm
ls test-output/cmd/ConvertCmdTest/jakarta_raya_coastline.osm
