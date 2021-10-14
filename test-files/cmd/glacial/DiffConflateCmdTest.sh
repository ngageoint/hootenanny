#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/glacial/DiffConflateCmdTest
OUTPUT_DIR=test-output/cmd/glacial/DiffConflateCmdTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn
CONFIG="-C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf"
# When using Diff conflate w/ Network (everything in here is using Network), we're not allowed to 
# remove partial match elements in a partial fashion at this time, so change the default setting to 
# avoid a warning.
GENERAL_OPTS="-D differential.remove.linear.partial.matches.as.whole=true"
source scripts/core/ScriptTestUtils.sh

# Run differential conflation to produce a map output
echo ""
echo "Running diff..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
  $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/output.osm --differential
if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUTPUT_DIR/output.osm \
    --report-output $OUTPUT_DIR/output-validation-report --output $OUTPUT_DIR/output-validated.osm
  diff $INPUT_DIR/output-validation-report $OUTPUT_DIR/output-validation-report
fi
validateTestOutput $OUTPUT_DIR/output.osm \
  $OUTPUT_DIR/snapped-with-ref-output-validation-report \
  $OUTPUT_DIR/output-validated.osm \
  $INPUT_DIR/output-validation-report

# Run changeset w/tags to produce a unified changeset output (geometry and tags)
echo ""
echo "Running diff changeset with tags..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS  \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output_unified.osc --differential --include-tags --changeset-stats $OUTPUT_DIR/output_unified_changeset_stats.json
# Check command line display of stats
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS  \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output_unified.osc --differential --include-tags --changeset-stats

# Run changeset w/tags to produce a unified map (osm) output
echo ""
echo "Running diff with tags..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm $OUTPUT_DIR/output_unified.osm --differential --include-tags

# Run changeset w/tags to produce separate outputs for geometry and tags
echo ""
echo "Running diff changeset with tags, separate outputs..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output.osc --differential --include-tags --changeset-stats $OUTPUT_DIR/output_changeset_stats.json --separate-output
# Check command line display of stats
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output.osc --differential --include-tags --changeset-stats --separate-output

echo ""
echo "Checking differential output..."
echo ""
hoot diff --warn -C Testing.conf $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm || \
     diff $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm

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
  diff $OUTPUT_DIR/output.osc $INPUT_DIR/output.osc
fi

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

echo ""
echo "Checking unified geometry+tag diff changeset..."
echo ""
if diff $OUTPUT_DIR/output_unified.osc $INPUT_DIR/output_unified.osc >/dev/null ; then
  echo ""
  echo "Unified geometry+tag diff Changeset Files Match"
  echo ""
else
  echo ""
  echo "Unified geometry+tag diff Changeset Files Don't Match"
  echo ""
  diff $OUTPUT_DIR/output_unified.osc $INPUT_DIR/output_unified.osc
fi

echo ""
echo "Checking unified geometry+tag diff osm..."
echo ""
hoot diff --warn -C Testing.conf  $OUTPUT_DIR/output_unified.osm $INPUT_DIR/output_unified.osm || \
     diff $OUTPUT_DIR/output_unified.osm $INPUT_DIR/output_unified.osm

echo ""
echo "Checking Unified Changeset Stats..."
echo ""
if diff $OUTPUT_DIR/output_unified_changeset_stats.json $INPUT_DIR/output_unified_changeset_stats.json >/dev/null ; then
  echo ""
  echo "Changeset Unified Stats Files Match"
  echo ""
else
  echo ""
  echo "Changeset Unified Stats Files Don't Match"
  echo ""
  diff $OUTPUT_DIR/output_unified_changeset_stats.json $INPUT_DIR/output_unified_changeset_stats.json
fi

echo ""
echo "Checking Changeset Stats..."
echo ""
if diff $OUTPUT_DIR/output_changeset_stats.json $INPUT_DIR/output_changeset_stats.json >/dev/null ; then
  echo ""
  echo "Changeset Stats Files Match"
  echo ""
else
  echo ""
  echo "Changeset Stats Files Don't Match"
  echo ""
  diff $OUTPUT_DIR/output_changeset_stats.json $INPUT_DIR/output_changeset_stats.json
fi

echo ""
echo "Checking conflation of empty files..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG test-files/Empty.osm test-files/Empty.osm $OUTPUT_DIR/Empty.osm
hoot diff --warn -C Testing.conf test-files/Empty.osm $OUTPUT_DIR/Empty.osm || cat $OUTPUT_DIR/Empty.osm

echo ""
echo "Checking conflation with road snapping..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 -D writer.include.debug.tags=true -D differential.snap.unconnected.features=true \
 -D snap.unconnected.ways.snap.criteria=HighwayCriterion \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-output.osm --differential
hoot diff --warn -C Testing.conf $OUTPUT_DIR/snapped-output.osm $INPUT_DIR/snapped-output.osm || \
  diff $OUTPUT_DIR/snapped-output.osm $INPUT_DIR/snapped-output.osm

echo ""
echo "Checking conflation with road snapping and w/ tags..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 -D writer.include.debug.tags=true -D differential.snap.unconnected.features=true \
 -D snap.unconnected.ways.snap.criteria=HighwayCriterion \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-with-tags-output.osm --differential --include-tags
hoot diff --warn -C Testing.conf $OUTPUT_DIR/snapped-with-tags-output.osm $INPUT_DIR/snapped-with-tags-output.osm || \
  diff $OUTPUT_DIR/snapped-with-tags-output.osm $INPUT_DIR/snapped-with-tags-output.osm

echo ""
echo "Checking conflation with road snapping and keeping ref data..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 -D writer.include.debug.tags=true -D differential.snap.unconnected.features=true \
 -D snap.unconnected.ways.snap.criteria=HighwayCriterion \
 -D differential.remove.reference.data=false \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-with-ref-output.osm --differential
hoot diff --warn -C Testing.conf $OUTPUT_DIR/snapped-with-ref-output.osm \
  $INPUT_DIR/snapped-with-ref-output.osm || \
  diff $OUTPUT_DIR/snapped-with-ref-output.osm $INPUT_DIR/snapped-with-ref-output.osm
if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $OUTPUT_DIR/snapped-with-ref-output.osm \
    --report-output $OUTPUT_DIR/snapped-with-ref-output-validation-report --output \
    $OUTPUT_DIR/snapped-with-ref-output-validated.osm
  diff $INPUT_DIR/snapped-with-ref-output-validation-report $OUTPUT_DIR/snapped-with-ref-output-validation-report
fi
validateTestOutput $OUTPUT_DIR/snapped-with-ref-output.osm \
  $OUTPUT_DIR/snapped-with-ref-output-validation-report \
  $OUTPUT_DIR/snapped-with-ref-output-validated.osm \
  $INPUT_DIR/snapped-with-ref-output-validation-report
     
echo ""
echo "Checking conflation with road snapping and remove all ref data, even snapped..."
echo ""
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 -D writer.include.debug.tags=true -D differential.snap.unconnected.features=true \
 -D snap.unconnected.ways.snap.criteria=HighwayCriterion \
 -D differential.remove.reference.snapped.data=true \
 $INPUT_DIR/input3.osm $INPUT_DIR/input4.osm \
 $OUTPUT_DIR/snapped-with-all-ref-removed-output.osm --differential
hoot diff --warn -C Testing.conf $OUTPUT_DIR/snapped-with-all-ref-removed-output.osm \
  $INPUT_DIR/snapped-with-all-ref-removed-output.osm || \
  diff $OUTPUT_DIR/snapped-with-all-ref-removed-output.osm $INPUT_DIR/snapped-with-all-ref-removed-output.osm

echo ""
echo "Running diff and passing unconflatable data through to output..."
echo ""
# The only difference you'll see here between this and the first diff conflate execution is that the 
# retaining wall from the second dataset, which hoot is unable to conflate, will pass through to output.
hoot conflate $LOG_LEVEL $CONFIG $GENERAL_OPTS \
 -D differential.remove.unconflatable.data=false $INPUT_DIR/input1.osm $INPUT_DIR/input2.osm \
 $OUTPUT_DIR/output-keep-unconflatable.osm --differential
hoot diff --warn -C Testing.conf $OUTPUT_DIR/output-keep-unconflatable.osm $INPUT_DIR/output-keep-unconflatable.osm || \
     diff $OUTPUT_DIR/output-keep-unconflatable.osm $INPUT_DIR/output-keep-unconflatable.osm
