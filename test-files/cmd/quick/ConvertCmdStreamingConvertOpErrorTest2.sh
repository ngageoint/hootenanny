#!/bin/bash
set -e

# pass in more than one convert op to a streaming convert; should get an error
hoot convert -D writer.xml.sort.by.id=false -D convert.ops="hoot::PoiCriterion;hoot::BuildingCriterion" test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.osm


