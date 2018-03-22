#!/bin/bash
set -e

mkdir -p test-output/NonDestructiveTest/

# First run the Congo network conflation
hoot conflate -C core/Network.conf -D reader.conflate.use.data.source.ids.1=true \
 test-files/Congo_MGCP_Roads_Bridges_subset.osm \
 test-files/Congo_OSM_Roads_Bridges_subset.osm \
 test-output/NonDestructiveTest/output.osm


# Derive the changeset for the Congo conflation
hoot derive-changeset --stats \
 test-files/Congo_MGCP_Roads_Bridges_subset.osm \
 test-output/NonDestructiveTest/output.osm \
 test-output/NonDestructiveTest/changeset.osc

# Old stats
# ---------
#
# Data Counts
#
# |        | Node | Relation | Way |
# |  REF1  | 8126 |        1 |  82 |
# |  REF2  | 1032 |        0 | 218 |
#
# @b2ef060
#
# |        | Node | Relation | Way |
# | Create |  856 |        0 | 900 |
# | Delete |    2 |        1 |  59 |
# | Modify |    0 |        0 |   4 |
#
# @bb953fb
#
# |        | Node | Relation | Way |
# | Create |  859 |        0 | 860 |
# | Delete |    1 |        1 |  17 |
# | Modify |    0 |        0 |  45 |
#
