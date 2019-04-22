#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/DiffConflateCmdTest

# Run differential conflation to produce a map output
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 test-files/conflate/diff/input1.osm test-files/conflate/diff/input2.osm \
 test-output/cmd/slow/DiffConflateCmdTest/output.osm --differential

# Run changeset w/tags to produce a unified changeset output (geometry and tags)
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 test-files/conflate/diff/input1.osm test-files/conflate/diff/input2.osm \
 test-output/cmd/slow/DiffConflateCmdTest/output_unified.osc --differential --include-tags

# Run changeset w/tags to produce a unified map (osm) output
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 test-files/conflate/diff/input1.osm test-files/conflate/diff/input2.osm \
 test-output/cmd/slow/DiffConflateCmdTest/output_unified.osm --differential --include-tags

# Run changeset w/tags to produce separate outputs for geometry and tags
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 test-files/conflate/diff/input1.osm test-files/conflate/diff/input2.osm \
 test-output/cmd/slow/DiffConflateCmdTest/output.osc --differential --include-tags --separate-output

# Check differential output
echo "Checking differential output"
hoot diff -C Testing.conf --warn test-output/cmd/slow/DiffConflateCmdTest/output.osm test-files/cmd/slow/DiffConflateCmdTest/output.osm || \
     diff test-output/cmd/slow/DiffConflateCmdTest/output.osm test-files/cmd/slow/DiffConflateCmdTest/output.osm

# Check changeset output
echo "Checking Changeset"
if diff test-output/cmd/slow/DiffConflateCmdTest/output.osc test-files/cmd/slow/DiffConflateCmdTest/output.osc >/dev/null ; then
  echo "Changeset Files Match"
else
  echo "Changeset Files Don't Match"
  diff test-output/cmd/slow/DiffConflateCmdTest/output.osc test-files/cmd/slow/DiffConflateCmdTest/output.osc
fi

# Check tag output
echo "Checking tag diff"
if diff test-output/cmd/slow/DiffConflateCmdTest/output.tags.osc test-files/cmd/slow/DiffConflateCmdTest/output.tags.osc >/dev/null ; then
  echo "Tag Changeset Files Match"
else
  echo "Tag Changeset Files Don't Match"
  diff test-output/cmd/slow/DiffConflateCmdTest/output.tags.osc test-files/cmd/slow/DiffConflateCmdTest/output.tags.osc
fi

# Check unified changeset output
echo "Checking unified geometry+tag diff changeset"
if diff test-output/cmd/slow/DiffConflateCmdTest/output_unified.osc test-files/cmd/slow/DiffConflateCmdTest/output_unified.osc >/dev/null ; then
  echo "Unified Changeset Files Match"
else
  echo "Unified Changeset Files Don't Match"
  diff test-output/cmd/slow/DiffConflateCmdTest/output_unified.osc test-files/cmd/slow/DiffConflateCmdTest/output_unified.osc
fi

# Check unified osm output
echo "Checking unified geometry+tag diff osm"
hoot diff -C Testing.conf --warn test-output/cmd/slow/DiffConflateCmdTest/output_unified.osm test-files/cmd/slow/DiffConflateCmdTest/output_unified.osm || \
     diff test-output/cmd/slow/DiffConflateCmdTest/output_unified.osm test-files/cmd/slow/DiffConflateCmdTest/output_unified.osm

# Check to make sure we don't bomb out on empty files
hoot conflate -C Testing.conf -C DifferentialConflation.conf -C NetworkAlgorithm.conf --warn test-files/Empty.osm test-files/Empty.osm tmp/DiffConflateCmdTest.osm
hoot diff -C Testing.conf test-files/Empty.osm tmp/DiffConflateCmdTest.osm || cat tmp/DiffConflateCmdTest.osm
