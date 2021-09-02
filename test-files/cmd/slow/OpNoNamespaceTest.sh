#!/bin/bash
set -e

# This tests the usage of hoot operators in certain situations without specifying the root namespace 
# () before the operator class name. The commands should behave the same without the namespace 
# specified. Prepending of the namespace happens automatically in the Settings class when an option 
# value is retrieved. So, we simply test both an example of a string and a string list option here.

IN_DIR=$HOOT_HOME/test-files/cmd/slow/OpNoNamespaceTest
OUT_DIR=$HOOT_HOME/test-output/cmd/slow/OpNoNamespaceTest
mkdir -p $OUT_DIR
LOG_LEVEL="--warn"
CONFIG="-C ReferenceConflation.conf -C UnifyingAlgorithm.conf -C Testing.conf"

# conflate
hoot conflate $LOG_LEVEL $CONFIG -D match.creators="HighwayMatchCreator" -D merger.creators="HighwayMergerCreator" -D conflate.pre.ops="RemoveMissingElementsVisitor;MapCleaner;RubberSheet" -D conflate.post.ops="RemoveMissingElementsVisitor;DuplicateWayRemover" test-files/ToyTestA.osm test-files/ToyTestB.osm $OUT_DIR/conflate-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/conflate-out.osm $OUT_DIR/conflate-out.osm

# convert
hoot convert $LOG_LEVEL $CONFIG -D convert.ops="FindHighwayIntersectionsOp" test-files/DcGisRoads.osm $OUT_DIR/convert-out.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/convert-out.osm $OUT_DIR/convert-out.osm
