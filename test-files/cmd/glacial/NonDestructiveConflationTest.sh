#!/bin/bash
set -e

mkdir -p test-output/cmd/glacial/NonDestructiveTest/

# First run the Congo network conflation
hoot conflate -C Testing.conf -C ReferenceConflation.conf -C NetworkAlgorithm.conf \
 -D match.creators=NetworkMatchCreator -D merger.creators=NetworkMergerCreator \
 -D writer.include.debug.tags=true \
 -D conflate.post.ops++=UnconnectedWaySnapper -D snap.unconnected.ways.favor.reference.way.node=true \
 test-files/Congo_MGCP_Roads_Bridges_subset.osm \
 test-files/Congo_OSM_Roads_Bridges_subset.osm \
 test-output/cmd/glacial/NonDestructiveTest/output.osm

# Derive the changeset for the Congo conflation
hoot changeset-derive -C Testing.conf --stats \
 test-files/Congo_MGCP_Roads_Bridges_subset.osm \
 test-output/cmd/glacial/NonDestructiveTest/output.osm \
 test-output/cmd/glacial/NonDestructiveTest/changeset.osc

# Check the output against the expected output
hoot diff -C Testing.conf \
  test-output/cmd/glacial/NonDestructiveTest/output.osm \
  test-files/cmd/glacial/NonDestructiveTest/Expected.osm || \
  diff \
  test-output/cmd/glacial/NonDestructiveTest/output.osm \
  test-files/cmd/glacial/NonDestructiveTest/Expected.osm

# Old stats
# ---------
#
# Data Counts
#
# |        | Node | Relation | Way |
# |  REF1  | 8126 |        1 |  82 |
# |  REF2  | 1032 |        0 | 218 |
#
# @b2ef060 - Baseline
#
# |        | Node | Relation | Way |
# | Create |  856 |        0 | 900 |
# | Delete |    2 |        1 |  59 |
# | Modify |    0 |        0 |   4 |
#
# @ - GDAL, GEOS upgrades
#
# |        | Node | Relation | Way |
# | Create |  873 |        0 | 244 |
# | Delete |    2 |        1 |  15 |
# | Modify |    0 |        0 |  10 |
#
