#!/bin/bash
set -e

mkdir -p test-output/cmd/ConflateCmdHighwayExactMatchInputsTest
cp test-files/DcTigerRoads.osm test-files/DcTigerRoadsHighwayExactMatchInputs.osm

hoot conflate -D "writer.include.debug=true" -D "conflate.add.score.tags=true" -D "uuid.helper.repeatable=true" test-files/DcTigerRoads.osm test-files/DcTigerRoadsHighwayExactMatchInputs.osm test-output/cmd/ConflateCmdHighwayExactMatchInputsTest/output.osm

hoot is-match test-output/cmd/ConflateCmdHighwayExactMatchInputsTest/output.osm test-files/cmd/slow/ConflateCmdHighwayExactMatchInputsTest/output.osm || diff test-output/cmd/ConflateCmdHighwayExactMatchInputsTest/output.osm test-files/cmd/slow/ConflateCmdHighwayExactMatchInputsTest/output.osm
