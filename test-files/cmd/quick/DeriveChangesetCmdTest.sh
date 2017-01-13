#!/bin/bash
set -e

mkdir -p test-output/cmd/DeriveChangesetCmdTest

hoot derive-changeset test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset.osc

