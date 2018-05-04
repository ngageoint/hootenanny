#!/bin/bash
set -e

# rnd command
OPT_COMMAND=`hoot | grep generate-alpha-shape`
if [ -z "$OPT_COMMAND" ]; then
 exit 0
fi

mkdir -p test-output/cmd/GenerateAlphaShapeCmdTest

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 0 test-output/cmd/GenerateAlphaShapeCmdTest/output-1.osm
hoot map-diff test-output/cmd/GenerateAlphaShapeCmdTest/output-1.osm test-files/cmd/slow/GenerateAlphaShapeCmdTest/output-1.osm || diff test-output/cmd/GenerateAlphaShapeCmdTest/output-1.osm test-files/cmd/slow/GenerateAlphaShapeCmdTest/output-1.osm 

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 1000 test-output/cmd/GenerateAlphaShapeCmdTest/output-2.osm
hoot map-diff test-output/cmd/GenerateAlphaShapeCmdTest/output-2.osm test-files/cmd/slow/GenerateAlphaShapeCmdTest/output-2.osm || diff test-output/cmd/GenerateAlphaShapeCmdTest/output-2.osm test-files/cmd/slow/GenerateAlphaShapeCmdTest/output-2.osm 

hoot generate-alpha-shape test-files/DcGisRoads.osm 1000 -500 test-output/cmd/GenerateAlphaShapeCmdTest/output-3.osm
hoot map-diff test-output/cmd/GenerateAlphaShapeCmdTest/output-3.osm test-files/cmd/slow/GenerateAlphaShapeCmdTest/output-3.osm || diff test-output/cmd/GenerateAlphaShapeCmdTest/output-3.osm test-files/cmd/slow/GenerateAlphaShapeCmdTest/output-3.osm
