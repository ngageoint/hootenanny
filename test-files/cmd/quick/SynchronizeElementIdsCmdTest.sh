#!/bin/bash

# The tag, hoot:synced:id=yes, can be checked to see if an element's ID was synced.

LOG="--status"
CONF="-C Testing.conf -D writer.include.debug.tags=true"
IN=test-files/cmd/quick/SynchronizeElementIdsCmdTest
OUT=test-output/cmd/quick/SynchronizeElementIdsCmdTest
mkdir -p $OUT

# Write out a copy of two maps combined without their element IDs preserved.
hoot convert $LOG $CONF -D reader.use.data.source.ids=false test-files/ToyTestA.osm test-files/ToyTestB.osm $OUT/in2.osm

# Synchronize the element IDs of the second map with that of the first map (overwrite the second map's element IDs with
# matching elements from the first map). Use as the first input another map which happens to have some of its data 
# identical to that of the map written in the previous step. IDs for this map are preserved by default upon load. For the 
# second input, use the map written in the previous step that doesn't have its IDs preserved. The output should consist of 
# the second map with some of its original IDs overwritten by those from the first input. The only elements in the output 
# with overwritten IDs should be those that matched with an element from the first input.
hoot sync-element-ids $LOG $CONF test-files/conflate/unified/AllDataTypesA.osm $OUT/in2.osm $OUT/out.osm

hoot diff --warn $CONF $IN/out.osm $OUT/out.osm
