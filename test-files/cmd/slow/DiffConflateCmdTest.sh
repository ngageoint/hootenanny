#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/slow/DiffConflateCmdTest
OUTPUT_DIR=test-output/cmd/slow/DiffConflateCmdTest
mkdir -p $OUTPUT_DIR

# Run differential conflation to produce a map output
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output.osm --differential

# Run changeset w/tags to produce a unified changeset output (geometry and tags)
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output_unified.osc --differential --include-tags

# Run changeset w/tags to produce a unified map (osm) output
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output_unified.osm --differential --include-tags

# Run changeset w/tags to produce separate outputs for geometry and tags
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output.osc --differential --include-tags --separate-output

# Check differential output
echo "Checking differential output"
hoot diff -C Testing.conf --warn $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm || \
     diff $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm

# Check changeset output
echo "Checking Changeset"
if diff $OUTPUT_DIR/output.osc $INPUT_DIR/output.osc >/dev/null ; then
  echo "Changeset Files Match"
else
  echo "Changeset Files Don't Match"
  diff $OUTPUT_DIR/output.osc t$INPUT_DIR/output.osc
fi

# Check tag output
echo "Checking tag diff"
if diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc >/dev/null ; then
  echo "Tag Changeset Files Match"
else
  echo "Tag Changeset Files Don't Match"
  diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc
fi

# Check unified changeset output
echo "Checking unified geometry+tag diff changeset"
if diff $OUTPUT_DIR/output_unified.osc $INPUT_DIR/output_unified.osc >/dev/null ; then
  echo "Unified Changeset Files Match"
else
  echo "Unified Changeset Files Don't Match"
  diff $OUTPUT_DIR/output_unified.osc $INPUT_DIR/output_unified.osc
fi

# Check unified osm output
echo "Checking unified geometry+tag diff osm"
hoot diff -C Testing.conf --warn $OUTPUT_DIR/output_unified.osm $INPUT_DIR/output_unified.osm || \
     diff $OUTPUT_DIR/output_unified.osm $INPUT_DIR/output_unified.osm

# Check to make sure we don't bomb out on empty files
hoot conflate -C Testing.conf -C DifferentialConflation.conf -C NetworkAlgorithm.conf --warn test-files/Empty.osm test-files/Empty.osm tmp/DiffConflateCmdTest.osm
hoot diff -C Testing.conf test-files/Empty.osm tmp/DiffConflateCmdTest.osm || cat tmp/DiffConflateCmdTest.osm

# Run with the road snapper
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 -D differential.snap.unconnected.roads=true \
 -D snap.unconnected.ways.snap.to.way.criterion=hoot::HighwayCriterion \
 -D snap.unconnected.ways.snap.to.way.node.criterion=hoot::HighwayNodeCriterion \
 -D snap.unconnected.ways.snap.way.criterion=hoot::HighwayCriterion -D snap.unconnected.ways.snap.way.status=Input2 \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-output.osm --differential
hoot diff -C Testing.conf --warn $OUTPUT_DIR/snapped-output.osm $INPUT_DIR/snapped-output.osm || \
     diff $OUTPUT_DIR/snapped-output.osm $INPUT_DIR/snapped-output.osm

# Run with the road snapper and keep the ref data
hoot conflate -C Testing.conf --warn -C DifferentialConflation.conf -C NetworkAlgorithm.conf \
 -D differential.snap.unconnected.roads=true -D differential.remove.reference.data=false \
 -D snap.unconnected.ways.snap.to.way.criterion=hoot::HighwayCriterion \
 -D snap.unconnected.ways.snap.to.way.node.criterion=hoot::HighwayNodeCriterion \
 -D snap.unconnected.ways.snap.way.criterion=hoot::HighwayCriterion -D snap.unconnected.ways.snap.way.status=Input2 \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-with-ref-output.osm --differential
hoot diff -C Testing.conf --warn $OUTPUT_DIR/snapped-with-ref-output.osm $INPUT_DIR/snapped-with-ref-output.osm || \
     diff $OUTPUT_DIR/snapped-with-ref-output.osm $INPUT_DIR/snapped-with-ref-output.osm
