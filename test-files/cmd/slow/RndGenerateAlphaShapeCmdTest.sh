#!/bin/bash
set -e

mkdir -p test-output/cmd/RndGenerateAlphaShapeCmdTest

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 0 test-output/cmd/RndGenerateAlphaShapeCmdTest/output-1.osm
hoot map-diff test-output/cmd/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-1.osm || diff test-output/cmd/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-1.osm 

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 1000 test-output/cmd/RndGenerateAlphaShapeCmdTest/output-2.osm
hoot map-diff test-output/cmd/RndGenerateAlphaShapeCmdTest/output-2.osm test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-2.osm || diff test-output/cmd/RndGenerateAlphaShapeCmdTest/output-2.osm test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-2.osm 

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 -500 test-output/cmd/RndGenerateAlphaShapeCmdTest/output-3.osm
hoot map-diff test-output/cmd/RndGenerateAlphaShapeCmdTest/output-3.osm test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-3.osm || diff test-output/cmd/RndGenerateAlphaShapeCmdTest/output-3.osm test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-3.osm
