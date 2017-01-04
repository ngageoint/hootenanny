#!/bin/bash
set -e

mkdir -p test-output/cmd/ConflateCmdFrechetExactMatchInputsTest
cp test-files/DcTigerRoads.osm test-files/DcTigerRoadsFrechetInput.osm

hoot conflate -D "writer.include.debug=true" -D "conflate.add.score.tags=true" -D "uuid.helper.repeatable=true" -D "way.subline.matcher=hoot::FrechetSublineMatcher" test-files/DcTigerRoads.osm test-files/DcTigerRoadsFrechetInput.osm test-output/cmd/ConflateCmdFrechetExactMatchInputsTest/output.osm

hoot is-match test-output/cmd/ConflateCmdFrechetExactMatchInputsTest/output.osm test-files/cmd/slow/ConflateCmdFrechetExactMatchInputsTest/output.osm || diff test-output/cmd/ConflateCmdFrechetExactMatchInputsTest/output.osm test-files/cmd/slow/ConflateCmdFrechetExactMatchInputsTest/output.osm
