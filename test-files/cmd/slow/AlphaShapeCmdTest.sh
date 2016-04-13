#!/bin/bash
set -e

mkdir -p test-output/cmd/AlphaShapeCmdTest

hoot alpha-shape test-files/DcGisRoads.osm 1000 0 test-output/cmd/AlphaShapeCmdTest/output-1.osm
hoot is-match test-output/cmd/AlphaShapeCmdTest/output-1.osm test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm || diff test-output/cmd/AlphaShapeCmdTest/output-1.osm test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm 

hoot alpha-shape test-files/DcGisRoads.osm 1000 1000 test-output/cmd/AlphaShapeCmdTest/output-2.osm
hoot is-match test-output/cmd/AlphaShapeCmdTest/output-2.osm test-files/cmd/slow/AlphaShapeCmdTest/output-2.osm || diff test-output/cmd/AlphaShapeCmdTest/output-2.osm test-files/cmd/slow/AlphaShapeCmdTest/output-2.osm 

hoot alpha-shape test-files/DcGisRoads.osm 1000 -500 test-output/cmd/AlphaShapeCmdTest/output-3.osm
hoot is-match test-output/cmd/AlphaShapeCmdTest/output-3.osm test-files/cmd/slow/AlphaShapeCmdTest/output-3.osm || diff test-output/cmd/AlphaShapeCmdTest/output-3.osm test-files/cmd/slow/AlphaShapeCmdTest/output-3.osm
