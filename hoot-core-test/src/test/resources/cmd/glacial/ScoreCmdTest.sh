#!/bin/bash
set -e

baseMap1=$HOOT_HOME/test-files/DcGisRoads.osm
baseMap2=$HOOT_HOME/test-files/DcGisRoads.osm
uut=$HOOT_HOME/test-files/DcTigerRoads.osm

# Run the command. Note that we sanitize output to remove timestamps
# and some really small numbers 
hoot score $baseMap1 $baseMap2 $uut | \
           sed "s///g" | \
           sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g" | \
           sed "s/ [0-9]\+\.[0-9]\+e\-[0-9]\+//g"    

