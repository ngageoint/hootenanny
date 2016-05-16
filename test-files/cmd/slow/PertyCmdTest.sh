#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/PertyCmdTest

# Compare to known-good
inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
testfile=$HOOT_HOME/test-files/cmd/slow/PertyCmdTest/p_out.osm
outputfile=$HOOT_HOME/test-output/cmd/slow/PertyCmdTest/p_out.osm

# Run the command. Note that we sanitize output to remove 
# number of nodes reprojected, because this is variable
hoot perty -D perty.search.distance=20 \
           -D perty.way.generalize.probability=0.7 \
           $inputfile \
           $outputfile |\
           sed "s/^Reprojecting.*$//g"


# Checking for a match is not a good idea, because perturbation is random
# hoot is-match $testfile $outputfile || diff $testfile $outputfile
