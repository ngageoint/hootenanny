#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/PertyCmdTest

# Compare to known-good
inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
testfile=$HOOT_HOME/test-files/cmd/quick/PertyCmdTest/p_out.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/PertyCmdTest/p_out.osm

# Run the command. Note that we sanitize output to remove 
# number of nodes reprojected, because this is variable
hoot perty -C Testing.conf -D random.seed=1 -D perty.search.distance=20 \
           -D random.way.generalizer.probability=0.7 \
           $inputfile \
           $outputfile |\
           sed "s/^Reprojecting.*$//g"

hoot diff -C Testing.conf $testfile $outputfile || diff $testfile $outputfile
