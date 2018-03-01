#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/SortCmdTest

hoot sort --warn test-files/cmd/slow/SortCmdTest/input.geonames test-output/cmd/slow/SortCmdTest/output.geonames
hoot is-match --warn test-output/cmd/slow/SortCmdTest/output.geonames test-files/cmd/slow/SortCmdTest/output.geonames || diff test-output/cmd/slow/SortCmdTest/output.geonames test-files/cmd/slow/SortCmdTest/output.geonames

hoot sort --warn test-files/cmd/slow/SortCmdTest/input.osm test-output/cmd/slow/SortCmdTest/output.osm
hoot is-match --warn test-output/cmd/slow/SortCmdTest/output.osm test-files/cmd/slow/SortCmdTest/output.osm || diff test-output/cmd/slow/SortCmdTest/output.osm test-files/cmd/slow/SortCmdTest/output.osm

#TODO: add pbf; use pbf-is-sorted cmd too
