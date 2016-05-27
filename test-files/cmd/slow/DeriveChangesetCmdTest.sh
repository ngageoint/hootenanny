#!/bin/bash
set -e

mkdir -p test-output/cmd/DeriveChangesetCmdTest

hoot derive-changeset test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/DeriveChangesetCmdTest/changeset.osc
diff test-output/cmd/DeriveChangesetCmdTest/changeset.osc test-files/cmd/slow/DeriveChangesetCmdTest/changeset.osc

