#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/ConflateCmdTest

export CONFLATE_OPTS="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf -C Testing.conf"

hoot conflate $CONFLATE_OPTS test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm test-output/cmd/slow/ConflateCmdTest/output.osm
hoot diff -C Testing.conf test-output/cmd/slow/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm || diff test-output/cmd/slow/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm

# Check to make sure we don't bomb out on empty files
hoot conflate $CONFLATE_OPTS --warn test-files/Empty.osm test-files/Empty.osm tmp/ConflateCmdTest.osm
hoot diff -C Testing.conf test-files/Empty.osm tmp/ConflateCmdTest.osm || cat tmp/ConflateCmdTest.osm

# TODO: get this working
# Check to make sure we don't die when the legacy namespace is present on some classes.
#hoot conflate $CONFLATE_OPTS -D match.creators="hoot::HighwayMatchCreator" \
#  -D merger.creators="hoot::HighwayMergerCreator" -D conflate.post.ops+="hoot::RemoveTagsVisitor" \
#  -D tag.filter.keys="note" \
#  test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm \
#  test-output/cmd/slow/ConflateCmdTest/output2.osm
#hoot diff -C Testing.conf test-output/cmd/slow/ConflateCmdTest/output2.osm test-files/cmd/slow/ConflateCmdTest/output2.osm
