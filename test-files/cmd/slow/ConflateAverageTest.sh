#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/ConflateAverageTest
hoot --conflate -D conflate.enable.old.roads=true -D match.creators= test-files/DcGisRoads.osm test-files/DcTigerRoads.osm test-output/cmd/ConflateAverageTest/output.osm
hoot --is-match test-output/cmd/ConflateAverageTest/output.osm test-files/cmd/slow/ConflateAverageTest/output.osm || diff test-output/cmd/ConflateAverageTest/output.osm test-files/cmd/slow/ConflateAverageTest/output.osm

# Check to make sure the output file doesn't contain any reviewee
cat test-output/cmd/ConflateAverageTest/output.osm | grep '<tag k="type" v="review"/>' | wc -l
