#!/bin/bash
set -e

mkdir -p test-output/cmd/DeriveChangeCmdTest

hoot conflate test-files/ToyTestA.osm test-files/ToyTestB.osm test-output/cmd/DeriveChangeCmdTest/conflate-out.osm
hoot derive-change test-files/ToyTestA.osm test-output/cmd/DeriveChangeCmdTest/conflate-out.osm test-output/cmd/DeriveChangeCmdTest/changeset.osc
diff test-output/cmd/DeriveChangeCmdTest/changeset.osc test-files/cmd/slow/DeriveChangeCmdTest/changeset.osc 

