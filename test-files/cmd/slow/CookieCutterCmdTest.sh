#!/bin/bash
set -e

mkdir -p test-output/cmd/CookieCutterCmdTest

hoot cookie-cut test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/output-1.osm
hoot map-diff test-output/cmd/CookieCutterCmdTest/output-1.osm test-files/cmd/slow/CookieCutterCmdTest/output-1.osm || diff test-output/cmd/CookieCutterCmdTest/output-1.osm test-files/cmd/slow/CookieCutterCmdTest/output-1.osm 

hoot cookie-cut test-files/cmd/slow/RndGenerateAlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/output-2.osm --crop
hoot map-diff test-output/cmd/CookieCutterCmdTest/output-2.osm test-files/cmd/slow/CookieCutterCmdTest/output-2.osm || diff test-output/cmd/CookieCutterCmdTest/output-2.osm test-files/cmd/slow/CookieCutterCmdTest/output-2.osm 
