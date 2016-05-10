#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/ScoreMatchesCmdTest

# The DC roads might be a good test, but it takes about 20 min. to run...
#map1=$HOOT_HOME/test-files/DcGisRoads.osm
#map2=$HOOT_HOME/test-files/DcTigerRoads.osm

# Toy files take about 12 sec to run
map1=$HOOT_HOME/test-files/ToyTestA.osm
map2=$HOOT_HOME/test-files/ToyTestB.osm
outputfile=$HOOT_HOME/test-output/cmd/glacial/ScoreMatchesCmdTest/conflated.osm
#comparefile=$HOOT_HOME/hoot-core-test/src/test/resources/cmd/glacial/ScoreMatchesCmdTest/conflated.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot score-matches --confusion --optimize $map1 $map2 $outputfile |\
                   sed "s///g" |\
                   sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g" |\
                   sed "s/M took: [0-9]\+\.*[0-9]*s/M took: s/g" |\
                   sed "s/[0-9]\+\.[0-9]\+e\-[0-9]\+//g"


