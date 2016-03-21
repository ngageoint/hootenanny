#!/bin/bash
set -e

mkdir -p test-output/cmd/CookieCutterCmdTest

hoot cookie-cutter test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/output-1.osm
hoot is-match test-output/cmd/CookieCutterCmdTest/output-1.osm test-files/cmd/slow/CookieCutterCmdTest/output-1.osm || diff test-output/cmd/CookieCutterCmdTest/output-1.osm test-files/cmd/slow/CookieCutterCmdTest/output-1.osm 

hoot cookie-cutter test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/output-2.osm --crop
hoot is-match test-output/cmd/CookieCutterCmdTest/output-2.osm test-files/cmd/slow/CookieCutterCmdTest/output-2.osm || diff test-output/cmd/CookieCutterCmdTest/output-2.osm test-files/cmd/slow/CookieCutterCmdTest/output-2.osm 
