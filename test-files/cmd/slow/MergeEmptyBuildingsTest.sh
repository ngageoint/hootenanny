#!/bin/bash
set -e

# This tests the fix applied by #2034 by merely checking that no crash occurs due to the empty buildings that get passed into the building 
# merger.  The bug is only reproduced when Unifying conflation is used and all other matchers are activated (matchers circa Jan. 2018).

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/MergeEmptyBuildingsTest

hoot conflate --error -C Testing.conf -D uuid.helper.repeatable=true test-files/cmd/slow/MergeEmptyBuildingsTest/MapEditBandug-cropped-2.osm test-files/cmd/slow/MergeEmptyBuildingsTest/OSMmap-cropped-2.osm test-output/cmd/slow/MergeEmptyBuildingsTest/output.osm

