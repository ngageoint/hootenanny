#!/bin/bash
set -e

# This tests the fix applied by #2059 by merely checking that no crash occurs due to the bad match pairs created.  The bug
# is only reproduced when Network conflation is used and all other matchers are activated (Network conf and matchers 
# circa Jan. 2018).
# Finishing #2069 may require this test needing modification or may make it obsolete.

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/BadMatchPairTest

CONFIG="-C Testing.conf"

hoot conflate --error $CONFIG -D conflate.match.highway.classifier="hoot::HighwayExpertClassifier" -D way.subline.matcher="hoot::MaximalSublineMatcher" -D rubber.sheet.minimum.ties=4 -D rubber.sheet.ref=true -D uuid.helper.repeatable=true test-files/cmd/slow/BadMatchPairTest/MapEditBandug-cropped-1.osm test-files/cmd/slow/BadMatchPairTest/OSMmap-cropped-1.osm test-output/cmd/slow/BadMatchPairTest/output.osm

