#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/slow/DiffConflateTagChangeDifferingGeometriesTest
OUTPUT_DIR=test-output/cmd/slow/DiffConflateTagChangeDifferingGeometriesTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

# We run with the Network alg, which can generate node/way match pairs. Then we check to make sure 
# tag changes based on node/way match pairs don't exist in the tags changeset. Particularly, you 
# should not see any highway nodes in the output with the following tags transferred to them from 
# ways: access=permissive, highway=service, and surface=asphalt.

# Run changeset w/tags to produce separate outputs for geometry and tags. When using Diff conflate 
# w/ Network, we're not allowed to remove partial match elements in a partial fashion at this time, 
# so change the default setting to avoid a warning.
#
# Note: Have had difficulty running this with a reduced set of matchers for efficiency. Mostly b/c
# its tedious to compare the changeset output. The input data has so many feature types anyway that
# not a lot of efficiency will be gained by reducing the matchers.
echo "Running diff changeset with tags, separate outputs..."
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 -D differential.remove.linear.partial.matches.as.whole=true $INPUT_DIR/Input1.osm $INPUT_DIR/Input2.osm \
 $OUTPUT_DIR/output.osc --differential --include-tags --separate-output

# Check tag output
echo "Checking tag diff"
if diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc >/dev/null ; then
  echo "Tag Changeset Files Match"
else
  echo "Tag Changeset Files Don't Match"
  diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc
fi
