#!/bin/bash
set -e

CONFIG="-C Testing.conf"

# We just run the command and check stderr & stdout
# for correctitude
inputfile1=$HOOT_HOME/test-files/cmd/quick/TagCompareCmdTest/input.osm
inputfile2=$HOOT_HOME/test-files/cmd/quick/TagCompareCmdTest/input.osm
hoot tag-compare $CONFIG $inputfile1 $inputfile2

