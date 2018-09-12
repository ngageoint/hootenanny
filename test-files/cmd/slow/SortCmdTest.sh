#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/SortCmdTest

# don't have a geonames writer
hoot sort --warn test-files/cmd/slow/SortCmdTest/input.geonames test-output/cmd/slow/SortCmdTest/geonames-output.osm
hoot diff --warn test-output/cmd/slow/SortCmdTest/output.osm test-files/cmd/slow/SortCmdTest/geonames-output.osm || diff test-output/cmd/slow/SortCmdTest/geonames-output.osm test-files/cmd/slow/SortCmdTest/geonames-output.osm

hoot sort --warn test-files/cmd/slow/SortCmdTest/input.osm test-output/cmd/slow/SortCmdTest/osm-output.osm
hoot diff --warn test-output/cmd/slow/SortCmdTest/osm-output.osm test-files/cmd/slow/SortCmdTest/osm-output.osm || diff test-output/cmd/slow/SortCmdTest/osm-output.osm test-files/cmd/slow/SortCmdTest/osm-output.osm

hoot pbf-is-sorted test-files/cmd/slow/SortCmdTest/input.osm.pbf
hoot sort --warn test-files/cmd/slow/SortCmdTest/input.osm.pbf test-output/cmd/slow/SortCmdTest/output.osm.pbf
hoot diff --warn test-output/cmd/slow/SortCmdTest/output.osm.pbf test-files/cmd/slow/SortCmdTest/output.osm.pbf || diff test-output/cmd/slow/SortCmdTest/output.osm.pbf test-files/cmd/slow/SortCmdTest/output.osm.pbf
hoot pbf-is-sorted test-output/cmd/slow/SortCmdTest/output.osm.pbf
