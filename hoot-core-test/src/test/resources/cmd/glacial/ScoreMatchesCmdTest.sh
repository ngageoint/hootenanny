#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/ScoreMatchesCmdTest

# These files are kind of lame. Maybe make better test files in the future. 
map1=$HOOT_HOME/test-files/ScoreMatchRef1.osm
map2=$HOOT_HOME/test-files/ScoreMatchRef2.osm
outputfile=$HOOT_HOME/test-output/cmd/glacial/ScoreMatchesCmdTest/output.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot score-matches --confusion --optimize $map1 $map2 $outputfile 

#                   sed "s///g" |\
#                   sed "s/^[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}\.[0-9]\{3\} //g" |\
#                   sed "s/M took: [0-9]\+\.*[0-9]*s/M took: s/g" |\
#                   sed "s/[0-9]\+\.[0-9]\+e\-[0-9]\+//g" |\
#                   sed "s/Loading \(model\|highway model\) from: .*\$/Loading model/"

