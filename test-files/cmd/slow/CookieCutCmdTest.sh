#!/bin/bash
set -e

mkdir -p test-output/cmd/CookieCutCmdTest

hoot cookie-cut test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutCmdTest/output-1.osm
hoot diff test-output/cmd/CookieCutCmdTest/output-1.osm test-files/cmd/slow/CookieCutCmdTest/output-1.osm || diff test-output/cmd/CookieCutCmdTest/output-1.osm test-files/cmd/slow/CookieCutCmdTest/output-1.osm 

hoot cookie-cut test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutCmdTest/output-2.osm --crop
hoot diff test-output/cmd/CookieCutCmdTest/output-2.osm test-files/cmd/slow/CookieCutCmdTest/output-2.osm || diff test-output/cmd/CookieCutCmdTest/output-2.osm test-files/cmd/slow/CookieCutCmdTest/output-2.osm 
