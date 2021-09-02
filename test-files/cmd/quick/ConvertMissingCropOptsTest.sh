#!/bin/bash

# This should result in an error due to the crop.bounds config option missing.
hoot convert --warn -C Testing.conf -D convert.ops="MapCropper" test-files/DcTigerRoads.osm test-output/test.osm 
