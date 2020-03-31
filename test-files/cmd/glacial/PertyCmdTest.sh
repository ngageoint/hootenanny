#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/PertyCmdTest

# Compare to known-good
inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
testfile=$HOOT_HOME/test-files/cmd/glacial/PertyCmdTest/p_out.osm
outputfile=$HOOT_HOME/test-output/cmd/glacial/PertyCmdTest/p_out.osm

# Run the command. Note that we sanitize output to remove 
# number of nodes reprojected, because this is variable
hoot perty -C Testing.conf -D perty.search.distance=20 \
           -D random.way.generalizer.probability=0.7 \
           $inputfile \
           $outputfile |\
           sed "s/^Reprojecting.*$//g"

# TODO: use the random seeding feature in perty, then we can check for the match
# hoot diff -C Testing.conf $testfile $outputfile || diff $testfile $outputfile
