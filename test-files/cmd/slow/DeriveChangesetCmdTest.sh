#!/bin/bash
set -e

rm -rf test-output/cmd/DeriveChangesetCmdTest
mkdir -p test-output/cmd/DeriveChangesetCmdTest

# in memory sorting tests

hoot changeset-derive test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc
diff test-files/cmd/slow/DeriveChangesetCmdTest/changeset-1.osc test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc 

hoot changeset-derive test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc
diff test-files/cmd/slow/DeriveChangesetCmdTest/changeset-2.osc test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc 

# external merge sorting tests

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=osm test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset-3.osc
diff test-files/cmd/slow/DeriveChangesetCmdTest/changeset-1.osc test-output/cmd/DeriveChangesetCmdTest/changeset-3.osc 

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=osm test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/DeriveChangesetCmdTest/changeset-4.osc
diff test-files/cmd/slow/DeriveChangesetCmdTest/changeset-2.osc test-output/cmd/DeriveChangesetCmdTest/changeset-4.osc 

# The pbf writer uses the OpenStreetMap accuracy default of 100 nanodegrees.  So, the input data for this test will lose some precision when
# ExternalMergeElementSorter uses pbf temp files, as in the following tests.  Updating the pbf writer to support a higher resolution is 
# not a good idea, since no external tools would support the higher resolution.  Hoot actually uses a node coord comparison accuracy of 
# 100 nanodegrees places by default, so comparing the output of the following tests with the map diff command (MapComparator) would result 
# in passing tests.  However, the problem is that no such comparator exists for changesets.  Rather than spend time writing one and given 
# the fact that there is already a pbf format test in ExternalMergeElementSorterTest, decided not to verify output here and only see that 
# it doesn't fail during changeset generation.

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=pbf test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset-5.osc
#diff test-files/cmd/slow/DeriveChangesetCmdTest/changeset-1.osc test-output/cmd/DeriveChangesetCmdTest/changeset-5.osc

hoot changeset-derive -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=pbf test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/DeriveChangesetCmdTest/changeset-6.osc
#diff test-files/cmd/slow/DeriveChangesetCmdTest/changeset-2.osc test-output/cmd/DeriveChangesetCmdTest/changeset-6.osc 
