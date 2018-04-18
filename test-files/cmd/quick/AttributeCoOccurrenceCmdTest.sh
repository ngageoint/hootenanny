#!/bin/bash
set -e

# We just run the command and check stderr & stdout
# for correctitude
inputfile1=$HOOT_HOME/test-files/cmd/quick/AttributeCoOccurrenceTest/input.osm
inputfile2=$HOOT_HOME/test-files/cmd/quick/AttributeCoOccurrenceTest/input.osm
hoot attribute-co-occurrence $inputfile1 $inputfile2

