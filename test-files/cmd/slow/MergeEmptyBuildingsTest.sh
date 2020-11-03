#!/bin/bash
set -e

# This tests the fix applied by #2034 by merely checking that no crash occurs due to the empty buildings that get passed into the building 
# merger.  The bug is only reproduced when Unifying conflation is used and all other matchers are activated (matchers circa Jan. 2018).

mkdir -p $HOOT_HOME/tmp/
INPUT=test-files/cmd/slow/MergeEmptyBuildingsTest
OUTPUT=test-output/cmd/slow/MergeEmptyBuildingsTest
mkdir -p $OUTPUT

hoot conflate --error -C Testing.conf -D uuid.helper.repeatable=true $INPUT/MapEditBandug-cropped-2.osm $INPUT/OSMmap-cropped-2.osm $OUTPUT/output.osm

