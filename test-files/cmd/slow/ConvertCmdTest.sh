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

# Convert to SQL test
hoot convert \
        -D changeset.user.id=1359 \
        -D postgresql.dumpfile.writer.start.id.changeset=10000000000 \
        -D postgresql.dumpfile.writer.start.id.node=10000000000 \
        -D postgresql.dumpfile.writer.start.id.way=20000000000 \
        -D postgresql.dumpfile.writer.start.id.relation=30000000000 \
        -D postgresql.dumpfile.writer.user.email=featuretest@hoot.local \
        -D postgresql.dumpfile.writer.user.id=1359 \
	test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.sql

echo Check for invalid characters. You should see 3 sets of warnings.
hoot --convert  test-files/io/InvalidCharacters.osm test-output/cmd/ConvertCmdTest/InvalidCharacters.osm
