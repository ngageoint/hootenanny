#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/ScoreMatchesCmdTest

map1=$HOOT_HOME/test-files/DcGisRoads.osm
map2=$HOOT_HOME/test-files/DcTigerRoads.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/ScoreMatchesCmdTest/conflated.osm
comparefile=$HOOT_HOME/hoot-core-test/src/test/resources/cmd/slow/ScoreMatchesCmdTest/conflated.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot score-matches --confusion --optimize $map1 $map2 $outputfile |\
                   sed "s///g" |\
                   sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g" |\
                   sed "s/M took: [0-9]\+\.[0-9]\+s/M took: /g"

# Checking output
hoot is-match $comparefile $outputfile || diff $comparefile $outputfile
