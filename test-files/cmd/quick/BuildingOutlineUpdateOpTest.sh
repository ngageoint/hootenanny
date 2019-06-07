#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/BuildingOutlineUpdateOpTest

input1=$HOOT_HOME/test-files/cases/attribute/unifying/building-3022-duplicate-relation-member-ways-1/Input1.osm
input2=$HOOT_HOME/test-files/cases/attribute/unifying/building-3022-duplicate-relation-member-ways-1/Input2.osm
outputfile=$HOOT_HOME/test-output/cmd/quick/BuildingOutlineUpdateOpTest/BuildingOutlineUpdateOpTest.osm
comparefile=$HOOT_HOME/test-files/cmd/quick/BuildingOutlineUpdateOpTest/BuildingOutlineUpdateOpTest.osm

# Run the command.
hoot conflate -C Testing.conf $input1 $input2 $outputfile

# Check output
hoot diff $comparefile $outputfile || diff $comparefile $outputfile

