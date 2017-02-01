#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/NetworkConflateCmdTest

hoot conflate --warn -C Network.conf -D uuid.helper.repeatable=true -D writer.include.debug=true test-files/DcGisRoads.osm test-files/DcTigerRoads.osm test-output/cmd/NetworkConflateCmdTest/output.osm
hoot is-match test-output/cmd/NetworkConflateCmdTest/output.osm test-files/cmd/glacial/NetworkConflateCmdTest/output.osm || diff test-output/cmd/NetworkConflateCmdTest/output.osm test-files/cmd/glacial/NetworkConflateCmdTest/output.osm 

# Check to make sure we don't bomb out on empty files
hoot conflate --warn -C Network.conf -D uuid.helper.repeatable=true -D writer.include.debug=true test-files/Empty.osm test-files/Empty.osm tmp/dum.osm
hoot is-match test-files/Empty.osm tmp/dum.osm || cat tmp/dum.osm
