#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/ConflateCmdHighwayExactMatchInputsTest
cp test-files/DcTigerRoads.osm test-files/DcTigerRoadsHighwayExactMatchInputs.osm

CONFIG="-C Testing.conf"

hoot conflate $CONFIG -D match.creators="hoot::HighwayMatchCreator" -D merger.creators="hoot::HighwayMergerCreator" -D "writer.include.debug.tags=true" -D "writer.include.conflate.score.tags=true" -D "uuid.helper.repeatable=true" test-files/DcTigerRoads.osm test-files/DcTigerRoadsHighwayExactMatchInputs.osm test-output/cmd/slow/ConflateCmdHighwayExactMatchInputsTest/output.osm

hoot diff $CONFIG test-output/cmd/slow/ConflateCmdHighwayExactMatchInputsTest/output.osm test-files/cmd/slow/ConflateCmdHighwayExactMatchInputsTest/output.osm || diff test-output/cmd/slow/ConflateCmdHighwayExactMatchInputsTest/output.osm test-files/cmd/slow/ConflateCmdHighwayExactMatchInputsTest/output.osm
