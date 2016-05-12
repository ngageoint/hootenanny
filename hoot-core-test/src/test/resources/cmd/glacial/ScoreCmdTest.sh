#!/bin/bash
set -e

# DC roads take about 6 minutes to run
#baseMap1=$HOOT_HOME/test-files/DcGisRoads.osm
#baseMap2=$HOOT_HOME/test-files/DcGisRoads.osm
#uut=$HOOT_HOME/test-files/DcTigerRoads.osm

# Toy files take about 9 sec to run
baseMap1=$HOOT_HOME/test-files/ToyTestA.osm
baseMap2=$HOOT_HOME/test-files/ToyTestA.osm
uut=$HOOT_HOME/test-files/ToyTestB.osm

# Run the command. Note that we remove some really small numbers from
# the output, because they are essentially noise 
#hoot score $baseMap1 $baseMap2 $uut | \
#           sed "s///g" | \
#           sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g" | \
#           sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"    
hoot score $baseMap1 $baseMap2 $uut |\
           sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"    


