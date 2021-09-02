#!/bin/bash
set -e

# This tests the fix applied by #2059 by merely checking that no crash occurs due to the bad match pairs created.  The bug is only reproduced 
# when Network conflation is used and all other matchers are activated (Network conf and matchers circa Jan. 2018).

mkdir -p $HOOT_HOME/tmp/
INPUT=test-files/cmd/slow/BadMatchPairTest
OUTPUT=test-output/cmd/slow/BadMatchPairTest
mkdir -p $OUTPUT

CONFIG="-C Testing.conf"

hoot conflate --error $CONFIG -D conflate.match.highway.classifier="HighwayExpertClassifier" -D way.subline.matcher="MaximalSublineMatcher" -D rubber.sheet.minimum.ties=4 -D rubber.sheet.ref=true -D uuid.helper.repeatable=true $INPUT/MapEditBandug-cropped-1.osm $INPUT/OSMmap-cropped-1.osm $OUTPUT/output.osm

