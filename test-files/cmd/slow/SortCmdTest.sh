#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/SortCmdTest

hoot sort --warn test-files/cmd/slow/SortCmdTest/input.geonames test-output/cmd/slow/SortCmdTest/output.geonames
hoot map-diff --warn test-output/cmd/slow/SortCmdTest/output.geonames test-files/cmd/slow/SortCmdTest/output.geonames || diff test-output/cmd/slow/SortCmdTest/output.geonames test-files/cmd/slow/SortCmdTest/output.geonames

hoot sort --warn test-files/cmd/slow/SortCmdTest/input.osm test-output/cmd/slow/SortCmdTest/output.osm
hoot map-diff --warn test-output/cmd/slow/SortCmdTest/output.osm test-files/cmd/slow/SortCmdTest/output.osm || diff test-output/cmd/slow/SortCmdTest/output.osm test-files/cmd/slow/SortCmdTest/output.osm

hoot pbf-is-sorted test-files/cmd/slow/SortCmdTest/input.osm.pbf
hoot sort --warn test-files/cmd/slow/SortCmdTest/input.osm.pbf test-output/cmd/slow/SortCmdTest/output.osm.pbf
hoot map-diff --warn test-output/cmd/slow/SortCmdTest/output.osm.pbf test-files/cmd/slow/SortCmdTest/output.osm.pbf || diff test-output/cmd/slow/SortCmdTest/output.osm.pbf test-files/cmd/slow/SortCmdTest/output.osm.pbf
# Unfortunately, the sorted header isn't being properly updated yet.  So, this will return unsorted. - #2207
hoot pbf-is-sorted test-output/cmd/slow/SortCmdTest/output.osm.pbf
