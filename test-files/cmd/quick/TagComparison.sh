#!/bin/bash
set -e

# We just run the command and check stderr & stdout
# for correctitude
inputfile1=$HOOT_HOME/test-files/cmd/quick/TagComparisonTest/input.osm
inputfile2=$HOOT_HOME/test-files/cmd/quick/TagComparisonTest/input.osm
hoot tag-comparison $inputfile1 $inputfile2

