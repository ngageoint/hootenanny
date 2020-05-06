#!/bin/bash
set -e

# This tests the fix applied by #2034 by merely checking that no crash occurs due to the empty buildings that get 
# passed into the building merger.  The bug is only reproduced when Unifying conflation is used and all other 
# matchers are activated (matchers circa Jan. 2018).
# Finishing #2058 may require this test needing modification or may make it obsolete.

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/MergeEmptyBuildingsTest

hoot conflate --error -C Testing.conf -D match.creators="hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,Poi.js;hoot::HighwayMatchCreator;hoot::ScriptMatchCreator,River.js;hoot::PoiPolygonMatchCreator;hoot::ScriptMatchCreator,Area.js" -D merger.creators="hoot::BuildingMergerCreator;hoot::ScriptMergerCreator;hoot::HighwayMergerCreator;hoot::ScriptMergerCreator;hoot::PoiPolygonMergerCreator;hoot::ScriptMergerCreator" -D uuid.helper.repeatable=true test-files/cmd/slow/MergeEmptyBuildingsTest/MapEditBandug-cropped-2.osm test-files/cmd/slow/MergeEmptyBuildingsTest/OSMmap-cropped-2.osm test-output/cmd/slow/MergeEmptyBuildingsTest/output.osm

