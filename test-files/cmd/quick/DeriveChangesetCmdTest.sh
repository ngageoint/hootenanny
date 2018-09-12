#!/bin/bash
set -e

rm -rf test-output/cmd/DeriveChangesetCmdTest
mkdir -p test-output/cmd/DeriveChangesetCmdTest

# in memory sorting tests

hoot changeset-derive test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-1.osc

hoot changeset-derive test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-2.osc

# external merge sorting tests

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=osm test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-1.osc

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=osm test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-2.osc

# There are issues here with node coord precision when mixing the xml I/O used to read the soruce data with the temp pbf I/O used by 
# ExternalMergeElementSorter.  The pbf writer uses the OpenStreetMap accuracy default of 100 nanodegrees.  Hoot db and other writers by 
# default use a higher accuracy of .01 nano degrees (its configurable for the non-DB writers).  I toyed around with making the pbf 
# writer accuracy configurable but was unsuccessful.  I'm not convinced that making it configurable is a good idea anyway.  Doing so 
# is also not really needed anyway since (I think) pretty much any time we derive a changeset we are going to write it back to an OSM API db 
# and not a Hoot API db.  Hoot only uses a node coord comparison accuracy of seven decimal places, so actually comparing nodes 
# with the map diff command (MapComparator) will still pass here, but the problem is we're doing a straight file comparison and not 
# using a map diff comparison, since no such thing exists for changesets.  Rather than spend time writing one and given the fact that 
# there is already a pbf format test in ExternalMergeElementSorterTest, decided not to verify output here and only see that it doesn't 
# fail during generation.

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=pbf test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc
#diff test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-1.osc

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=pbf test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc
#diff test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-2.osc
