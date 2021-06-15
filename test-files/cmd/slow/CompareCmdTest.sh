#!/bin/bash
set -e

baseMap1=test-files/ToyTestA.osm
uut1=test-files/ToyTestB.osm

baseMap2=test-files/conflate/unified/AllDataTypesA.osm
uut2=test-files/conflate/unified/AllDataTypesB.osm

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

# Note that we remove some really small numbers from the output, because they are essentially noise. 
  
echo "Comparing single base map..."
echo ""
hoot compare $LOG_LEVEL $CONFIG $baseMap1 $uut1 | sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"
  
echo ""
echo "Comparing two base maps..."
echo ""
hoot compare $LOG_LEVEL $CONFIG $baseMap1 $baseMap1 $uut1 | sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"

echo ""  
echo "Comparing single base map with criteria..."
echo ""
hoot compare $LOG_LEVEL $CONFIG $baseMap1 $uut1 "HighwayCriterion" | sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"

echo ""  
echo "Comparing two base maps with criteria..."
echo ""
hoot compare $LOG_LEVEL $CONFIG $baseMap1 $baseMap1 $uut1 "HighwayCriterion" | sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g" 

echo ""
echo "Comparing two base maps with criteria and multiple threads..."
echo ""
hoot compare $LOG_LEVEL $CONFIG -D graph.comparator.max.threads=2 $baseMap1 $baseMap1 $uut1 \
  "HighwayCriterion" | sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"  
           
# Test with inputs that have something other than roads. Graph scoring takes a very long time for 
# these inputs, so we turn it off. Attribute scoring is returning an odd score for this and needs to 
# be looked into.
echo ""
echo "Comparing maps with multiple data types..."
echo ""
hoot compare $LOG_LEVEL $CONFIG $baseMap2 $uut2 --disable-graph --disable-attribute |\
  sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"
