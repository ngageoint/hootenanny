#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/CookieCutCmdTest

CONFIG="-C Testing.conf"

hoot cookie-cut $CONFIG test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/slow/CookieCutCmdTest/output-1.osm
hoot diff $CONFIG test-output/cmd/slow/CookieCutCmdTest/output-1.osm test-files/cmd/slow/CookieCutCmdTest/output-1.osm || diff test-output/cmd/slow/CookieCutCmdTest/output-1.osm test-files/cmd/slow/CookieCutCmdTest/output-1.osm

hoot cookie-cut $CONFIG test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/DcTigerRoads.osm test-output/cmd/slow/CookieCutCmdTest/output-2.osm --crop
hoot diff $CONFIG test-output/cmd/slow/CookieCutCmdTest/output-2.osm test-files/cmd/slow/CookieCutCmdTest/output-2.osm || diff test-output/cmd/slow/CookieCutCmdTest/output-2.osm test-files/cmd/slow/CookieCutCmdTest/output-2.osm
