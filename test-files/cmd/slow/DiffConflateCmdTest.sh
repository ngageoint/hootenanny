#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/DiffConflateCmdTest
hoot -C Network.conf --diff-conflate test-files/conflate/diff/me.osm test-files/conflate/diff/mgcp.osm test-output/cmd/DiffConflateCmdTest/output.osm

#hoot --is-match test-output/cmd/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm || diff test-output/cmd/ConflateCmdTest/output.osm test-files/cmd/slow/ConflateCmdTest/output.osm 

# Check to make sure we don't bomb out on empty files
#hoot conflate --warn test-files/Empty.osm test-files/Empty.osm tmp/dum.osm
#hoot --is-match test-files/Empty.osm tmp/dum.osm || cat tmp/dum.osm
