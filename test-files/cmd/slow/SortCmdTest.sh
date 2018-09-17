#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/SortCmdTest

BUFFER_SIZE=3

#There is no geonames writer, so we're writing that input to osm xml.

# in memory sorting tests

hoot sort --warn test-files/cmd/slow/SortCmdTest/input.geonames test-output/cmd/slow/SortCmdTest/geonames-output-1.osm
hoot diff --warn test-files/cmd/slow/SortCmdTest/geonames-output.osm test-output/cmd/slow/SortCmdTest/geonames-output-1.osm

hoot sort --warn test-files/cmd/slow/SortCmdTest/input.osm test-output/cmd/slow/SortCmdTest/osm-output-1.osm
hoot diff --warn test-files/cmd/slow/SortCmdTest/osm-output.osm test-output/cmd/slow/SortCmdTest/osm-output-1.osm

hoot pbf-is-sorted test-files/cmd/slow/SortCmdTest/input.osm.pbf
hoot sort --warn test-files/cmd/slow/SortCmdTest/input.osm.pbf test-output/cmd/slow/SortCmdTest/pbf-output-1.osm.pbf
hoot diff --warn test-files/cmd/slow/SortCmdTest/pbf-output.osm.pbf test-output/cmd/slow/SortCmdTest/pbf-output-1.osm.pbf
hoot pbf-is-sorted test-output/cmd/slow/SortCmdTest/pbf-output-1.osm.pbf

# external merge sorting tests

# The element not found warnings are expected here, as during the external sorting the temp files may have partial maps in them.  We just
# need to be sure the final output matches what is expected

hoot sort --warn -D element.sorter.element.buffer.size=$BUFFER_SIZE -D element.sorter.external.temp.format=osm test-files/cmd/slow/SortCmdTest/input.geonames test-output/cmd/slow/SortCmdTest/geonames-output-2.osm
hoot diff --warn test-files/cmd/slow/SortCmdTest/geonames-output.osm test-output/cmd/slow/SortCmdTest/geonames-output-2.osm

hoot sort --warn -D element.sorter.element.buffer.size=$BUFFER_SIZE -D element.sorter.external.temp.format=osm test-files/cmd/slow/SortCmdTest/input.osm test-output/cmd/slow/SortCmdTest/osm-output-2.osm
hoot diff --warn test-files/cmd/slow/SortCmdTest/osm-output.osm test-output/cmd/slow/SortCmdTest/osm-output-2.osm

hoot pbf-is-sorted test-files/cmd/slow/SortCmdTest/input.osm.pbf
hoot sort --warn -D element.sorter.element.buffer.size=$BUFFER_SIZE -D element.sorter.external.temp.format=osm test-files/cmd/slow/SortCmdTest/input.osm.pbf test-output/cmd/slow/SortCmdTest/pbf-output-2.osm.pbf
hoot diff --warn test-files/cmd/slow/SortCmdTest/pbf-output.osm.pbf test-output/cmd/slow/SortCmdTest/pbf-output-2.osm.pbf
hoot pbf-is-sorted test-output/cmd/slow/SortCmdTest/pbf-output-2.osm.pbf

hoot sort --warn -D element.sorter.element.buffer.size=$BUFFER_SIZE -D element.sorter.external.temp.format=pbf test-files/cmd/slow/SortCmdTest/input.geonames test-output/cmd/slow/SortCmdTest/geonames-output-3.osm
hoot diff --warn test-files/cmd/slow/SortCmdTest/geonames-output.osm test-output/cmd/slow/SortCmdTest/geonames-output-3.osm

# not working yet
hoot sort --warn -D element.sorter.element.buffer.size=$BUFFER_SIZE -D element.sorter.external.temp.format=pbf test-files/cmd/slow/SortCmdTest/input.osm test-output/cmd/slow/SortCmdTest/osm-output-3.osm
hoot diff --warn test-files/cmd/slow/SortCmdTest/osm-output.osm test-output/cmd/slow/SortCmdTest/osm-output-3.osm

hoot pbf-is-sorted test-files/cmd/slow/SortCmdTest/input.osm.pbf
hoot sort --warn -D element.sorter.element.buffer.size=$BUFFER_SIZE -D element.sorter.external.temp.format=pbf test-files/cmd/slow/SortCmdTest/input.osm.pbf test-output/cmd/slow/SortCmdTest/pbf-output-3.osm.pbf
hoot diff --warn test-files/cmd/slow/SortCmdTest/pbf-output.osm.pbf test-output/cmd/slow/SortCmdTest/pbf-output-3.osm.pbf
hoot pbf-is-sorted test-output/cmd/slow/SortCmdTest/pbf-output-3.osm.pbf
