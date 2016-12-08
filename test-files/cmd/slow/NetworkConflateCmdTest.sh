#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/NetworkConflateCmdTest

hoot conflate -C Network.conf test-files/DcGisRoads.osm test-files/DcTigerRoads.osm test-output/cmd/NetworkConflateCmdTest/output.osm
hoot is-match test-output/cmd/NetworkConflateCmdTest/output.osm test-files/cmd/slow/NetworkConflateCmdTest/output.osm || diff test-output/cmd/NetworkConflateCmdTest/output.osm test-files/cmd/slow/NetworkConflateCmdTest/output.osm 

# Check to make sure we don't bomb out on empty files
hoot conflate -C Network.conf --warn test-files/Empty.osm test-files/Empty.osm tmp/dum.osm
hoot is-match test-files/Empty.osm tmp/dum.osm || cat tmp/dum.osm
