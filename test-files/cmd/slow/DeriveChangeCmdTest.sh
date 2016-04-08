#!/bin/bash
set -e

mkdir -p test-output/cmd/DeriveChangeCmdTest

hoot derive-change test-files/ToyTestA.osm test-files/cmd/slow/DeriveChangeCmdTest/conflate-out.osm test-output/cmd/DeriveChangeCmdTest/changeset.osc
diff test-output/cmd/DeriveChangeCmdTest/changeset.osc test-files/cmd/slow/DeriveChangeCmdTest/changeset.osc 

