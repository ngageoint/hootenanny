#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/CookieCutCmdTest

hoot cookie-cut test-files/cmd/glacial/GenerateAlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/slow/CookieCutCmdTest/output-1.osm
hoot diff test-output/cmd/slow/CookieCutCmdTest/output-1.osm test-files/cmd/slow/CookieCutCmdTest/output-1.osm || diff test-output/cmd/slow/CookieCutCmdTest/output-1.osm test-files/cmd/slow/CookieCutCmdTest/output-1.osm

hoot cookie-cut test-files/cmd/glacial/GenerateAlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/slow/CookieCutCmdTest/output-2.osm --crop
hoot diff test-output/cmd/slow/CookieCutCmdTest/output-2.osm test-files/cmd/slow/CookieCutCmdTest/output-2.osm || diff test-output/cmd/slow/CookieCutCmdTest/output-2.osm test-files/cmd/slow/CookieCutCmdTest/output-2.osm
