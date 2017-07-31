#!/bin/bash
set -e

# pass in something other than an ElementCriterion to a streaming convert as a convert op; should get an error
hoot convert -D writer.xml.sort.by.id=false -D convert.ops="hoot::MapCropper" test-files/ToyTestA.osm.pbf test-output/cmd/ConvertCmdTest/ToyTestA.osm


