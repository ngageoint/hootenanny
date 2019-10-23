#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/ConflateCmdTest

export CONFLATE_OPTS="-C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf"

hoot conflate $CONFLATE_OPTS test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm test-output/cmd/slow/ConflateCmdTest/output.osm
hoot diff test-output/cmd/slow/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm || diff test-output/cmd/slow/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm

# Check to make sure we don't bomb out on empty files
hoot conflate $CONFLATE_OPTS --warn test-files/Empty.osm test-files/Empty.osm tmp/ConflateCmdTest.osm
hoot diff test-files/Empty.osm tmp/ConflateCmdTest.osm || cat tmp/ConflateCmdTest.osm
