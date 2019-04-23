#!/bin/bash
set -e

mkdir -p test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 0 test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-1.osm
hoot diff test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-1.osm || diff test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-1.osm

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 1000 test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-2.osm
hoot diff test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-2.osm test-files/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-2.osm || diff test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-2.osm test-files/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-2.osm

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 -500 test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-3.osm
hoot diff test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-3.osm test-files/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-3.osm || diff test-output/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-3.osm test-files/cmd/glacial/RndGenerateAlphaShapeCmdTest/output-3.osm
