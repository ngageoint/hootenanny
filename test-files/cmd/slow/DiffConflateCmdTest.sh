#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/DiffConflateCmdTest

# Check differential conflation
hoot conflate -C Diff.conf test-files/conflate/diff/input1.osm test-files/conflate/diff/input2.osm test-output/cmd/DiffConflateCmdTest/output.osm --differential
hoot map-diff test-output/cmd/DiffConflateCmdTest/output.osm test-files/cmd/slow/DiffConflateCmdTest/output.osm || diff test-output/cmd/DiffConflateCmdTest/output.osm test-files/cmd/slow/DiffConflateCmdTest/output.osm

# Check changeset derivation
hoot conflate -C Diff.conf test-files/conflate/diff/input1.osm test-files/conflate/diff/input2.osm test-output/cmd/DiffConflateCmdTest/output.osc --differential

if diff test-output/cmd/DiffConflateCmdTest/output.osc test-files/cmd/slow/DiffConflateCmdTest/output.osc >/dev/null ; then
  echo "Changeset Files Match"
else
  echo "Changeset Files Don't Match"
  diff test-output/cmd/DiffConflateCmdTest/output.osc test-files/cmd/slow/DiffConflateCmdTest/output.osc
fi

# Check to make sure we don't bomb out on empty files
hoot conflate --warn test-files/Empty.osm test-files/Empty.osm tmp/dum.osm
hoot map-diff test-files/Empty.osm tmp/dum.osm || cat tmp/dum.osm
