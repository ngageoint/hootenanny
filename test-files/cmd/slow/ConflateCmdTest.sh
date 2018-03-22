#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/ConflateCmdTest
hoot conflate test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm test-output/cmd/ConflateCmdTest/output.osm
hoot map-diff test-output/cmd/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm || diff test-output/cmd/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm

# Check to make sure we don't bomb out on empty files
hoot conflate --warn test-files/Empty.osm test-files/Empty.osm tmp/dum.osm
hoot map-diff test-files/Empty.osm tmp/dum.osm || cat tmp/dum.osm
