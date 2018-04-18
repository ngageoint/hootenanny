#!/bin/bash
set -e

rm -rf test-output/cmd/DeriveChangesetCmdTest
mkdir -p test-output/cmd/DeriveChangesetCmdTest

hoot changeset-derive test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset-1.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-1.osc

hoot changeset-derive test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset-2.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset-2.osc
