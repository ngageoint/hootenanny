#!/bin/bash
set -e

baseMap1=$HOOT_HOME/test-files/ToyTestA.osm
baseMap2=$HOOT_HOME/test-files/ToyTestA.osm
uut=$HOOT_HOME/test-files/ToyTestB.osm

# Run the command. Note that we remove some really small numbers from
# the output, because they are essentially noise    
hoot compare $baseMap1 $baseMap2 $uut |\
           sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"    


