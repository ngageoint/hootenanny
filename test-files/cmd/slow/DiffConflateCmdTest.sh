#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/slow/DiffConflateCmdTest
OUTPUT_DIR=test-output/cmd/slow/DiffConflateCmdTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

# Run differential conflation to produce a map output
echo ""
echo "Running diff..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output.osm --differential

# Run changeset w/tags to produce a unified changeset output (geometry and tags)
echo ""
echo "Running diff changeset with tags..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf  \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output_unified.osc --differential --include-tags

# Run changeset w/tags to produce a unified map (osm) output
echo ""
echo "Running diff with tags..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output_unified.osm --differential --include-tags

# Run changeset w/tags to produce separate outputs for geometry and tags
echo ""
echo "Running diff changeset with tags, separate outputs..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output.osc --differential --include-tags --separate-output

# Check differential output
echo ""
echo "Checking differential output..."
echo ""
hoot diff -C Testing.conf $LOG_LEVEL $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm || \
     diff $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm

# Check changeset output
echo ""
echo "Checking Changeset..."
echo ""
if diff $OUTPUT_DIR/output.osc $INPUT_DIR/output.osc >/dev/null ; then
  echo ""
  echo "Changeset Files Match"
  echo ""
else
  echo ""
  echo "Changeset Files Don't Match"
  echo ""
  diff $OUTPUT_DIR/output.osc t$INPUT_DIR/output.osc
fi

# Check tag output
echo ""
echo "Checking tag diff"
echo ""
if diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc >/dev/null ; then
  echo ""
  echo "Tag Changeset Files Match"
  echo ""
else
  echo ""
  echo "Tag Changeset Files Don't Match"
  echo ""
  diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc
fi

# Check unified changeset output
echo ""
echo "Checking unified geometry+tag diff changeset..."
echo ""
if diff $OUTPUT_DIR/output_unified.osc $INPUT_DIR/output_unified.osc >/dev/null ; then
  echo ""
  echo "Unified Changeset Files Match"
  echo ""
else
  echo ""
  echo "Unified Changeset Files Don't Match"
  echo ""
  diff $OUTPUT_DIR/output_unified.osc $INPUT_DIR/output_unified.osc
fi

# Check unified osm output
echo ""
echo "Checking unified geometry+tag diff osm..."
echo ""
hoot diff -C Testing.conf $LOG_LEVEL $OUTPUT_DIR/output_unified.osm $INPUT_DIR/output_unified.osm || \
     diff $OUTPUT_DIR/output_unified.osm $INPUT_DIR/output_unified.osm

# Check to make sure we don't bomb out on empty files
echo ""
echo "Checking conflation of empty files..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf --warn test-files/Empty.osm test-files/Empty.osm $OUTPUT_DIR/Empty.osm
hoot diff -C Testing.conf test-files/Empty.osm $OUTPUT_DIR/Empty.osm || cat $OUTPUT_DIR/Empty.osm

# Run with the road snapper
echo ""
echo "Checking conflation with road snapping..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 -D differential.snap.unconnected.roads=true \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-output.osm --differential
hoot diff -C Testing.conf --warn $OUTPUT_DIR/snapped-output.osm $INPUT_DIR/snapped-output.osm || \
     diff $OUTPUT_DIR/snapped-output.osm $INPUT_DIR/snapped-output.osm

# Run with the road snapper and keep the ref data
echo ""
echo "Checking conflation with road snapping and keeping ref data..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 -D differential.snap.unconnected.roads=true -D differential.remove.reference.data=false \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-with-ref-output.osm --differential
hoot diff -C Testing.conf $LOG_LEVEL $OUTPUT_DIR/snapped-with-ref-output.osm $INPUT_DIR/snapped-with-ref-output.osm || \
     diff $OUTPUT_DIR/snapped-with-ref-output.osm $INPUT_DIR/snapped-with-ref-output.osm

# Run differential conflation and keep unconflatable features - The only difference you'll see here between this and the first diff
# conflate execution is that the retaining wall from the second dataset, which hoot is unable to conflate, will pass through to output.
echo ""
echo "Running diff and passing unconflatable data through to output..."
echo ""
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 -D differential.remove.unconflatable.data=false $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output-keep-unconflatable.osm --differential
hoot diff -C Testing.conf $LOG_LEVEL $OUTPUT_DIR/output-keep-unconflatable.osm $INPUT_DIR/output-keep-unconflatable.osm || \
     diff $OUTPUT_DIR/output-keep-unconflatable.osm $INPUT_DIR/output-keep-unconflatable.osm
