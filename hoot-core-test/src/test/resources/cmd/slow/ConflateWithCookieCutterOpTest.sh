#!/bin/bash
set -e

mkdir -p test-output/cmd/CookieCutterCmdTest

hoot cookie-cutter test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/basic-output.osm
hoot is-match test-output/cmd/CookieCutterCmdTest/basic-output.osm test-files/cmd/slow/CookieCutterCmdTest/basic-output.osm || diff test-output/cmd/CookieCutterCmdTest/basic-output.osm test-files/cmd/slow/CookieCutterCmdTest/basic-output.osm 

hoot cookie-cutter test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/cropped-output.osm --crop
hoot is-match test-output/cmd/CookieCutterCmdTest/cropped-output.osm test-files/cmd/slow/CookieCutterCmdTest/cropped-output.osm || diff test-output/cmd/CookieCutterCmdTest/cropped-output.osm test-files/cmd/slow/CookieCutterCmdTest/cropped-output.osm 

hoot cookie-cutter test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/buffered-output.osm 500
hoot is-match test-output/cmd/CookieCutterCmdTest/buffered-output.osm test-files/cmd/slow/CookieCutterCmdTest/buffered-output.osm || diff test-output/cmd/CookieCutterCmdTest/buffered-output.osm test-files/cmd/slow/CookieCutterCmdTest/buffered-output.osm 

hoot cookie-cutter test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/CookieCutterCmdTest/cropped-and-buffered-output.osm 500 --crop
hoot is-match test-output/cmd/CookieCutterCmdTest/cropped-and-buffered-output test-files/cmd/slow/CookieCutterCmdTest/cropped-and-buffered-output.osm || diff test-output/cmd/CookieCutterCmdTest/cropped-and-buffered-output.osm test-files/cmd/slow/CookieCutterCmdTest/cropped-and-buffered-output.osm 
