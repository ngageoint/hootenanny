#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/AlphaShapeCmdTest

CONFIG="-C Testing.conf"

hoot alpha-shape $CONFIG --alpha 1000 --buffer 0 test-files/DcGisRoads.osm test-output/cmd/slow/AlphaShapeCmdTest/output-1.osm
hoot diff $CONFIG test-output/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm || diff test-output/cmd/slow/AlphaShapeCmdTest/output-1.osm test-files/cmd/slow/AlphaShapeCmdTest/output-1.osm

hoot alpha-shape $CONFIG --alpha 1000 --buffer 1000 test-files/DcGisRoads.osm test-output/cmd/slow/AlphaShapeCmdTest/output-2.osm
hoot diff $CONFIG test-output/cmd/slow/AlphaShapeCmdTest/output-2.osm test-files/cmd/slow/AlphaShapeCmdTest/output-2.osm || diff test-output/cmd/slow/AlphaShapeCmdTest/output-2.osm test-files/cmd/slow/AlphaShapeCmdTest/output-2.osm

hoot alpha-shape $CONFIG --alpha 1000 --buffer -500 test-files/DcGisRoads.osm test-output/cmd/slow/AlphaShapeCmdTest/output-3.osm
hoot diff $CONFIG test-output/cmd/slow/AlphaShapeCmdTest/output-3.osm test-files/cmd/slow/AlphaShapeCmdTest/output-3.osm || diff test-output/cmd/slow/AlphaShapeCmdTest/output-3.osm test-files/cmd/slow/AlphaShapeCmdTest/output-3.osm
