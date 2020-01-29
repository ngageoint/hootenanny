#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/ScoreMatchesCmdTest

# These files are kind of lame. Maybe make better test files in the future. 
map1=$HOOT_HOME/test-files/ScoreMatchRef1.osm
map2=$HOOT_HOME/test-files/ScoreMatchRef2.osm
outputfile=$HOOT_HOME/test-output/cmd/glacial/ScoreMatchesCmdTest/output.osm

# Run the command. Note that we sanitize output to remove timestamps
hoot score-matches -C Testing.conf --confusion --optimize $map1 $map2 $outputfile 


