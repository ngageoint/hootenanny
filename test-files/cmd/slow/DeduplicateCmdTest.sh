#!/bin/bash

LOG="--status"
CONF="-C Testing.conf -D writer.include.debug.tags=true"
IN=test-files/cmd/slow/DeduplicateCmdTest
OUT=test-output/cmd/slow/DeduplicateCmdTest
mkdir -p $OUT

# INTRA-MAP

# Write out a copy of two maps combined without their element IDs preserved. The resulting map will have some 
# duplicated elements.
hoot convert --warn $CONF -D reader.use.data.source.ids=false test-files/ToyTestA.osm \
  test-files/conflate/unified/AllDataTypesA.osm $OUT/in1.osm
# de-dupe the map
hoot de-duplicate $LOG $CONF $OUT/in1.osm $OUT/out-single-map.osm
hoot diff --warn $CONF $IN/out-single-map.osm $OUT/out-single-map.osm

# INTRA-MAP AND INTER-MAP

# Write out a copy of two maps combined without their element IDs preserved.
hoot convert --warn $CONF -D reader.use.data.source.ids=false test-files/ToyTestA.osm test-files/ToyTestB.osm $OUT/in2.osm
# This is the same as the previous test except we're also de-duping the second map input with itself.
hoot de-duplicate $LOG $CONF test-files/ToyTestA.osm $OUT/in2.osm $OUT/out-dual-map-1.osm $OUT/out-dual-map-2.osm
hoot diff --warn $CONF $IN/out-dual-map-1.osm $OUT/out-dual-map-1.osm
hoot diff --warn $CONF $IN/out-dual-map-2.osm $OUT/out-dual-map-2.osm
