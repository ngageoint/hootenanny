#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
INPUT_DIR=test-files/cmd/slow/DiffConflateTagChangeDifferingGeometriesTest
OUTPUT_DIR=test-output/cmd/slow/DiffConflateTagChangeDifferingGeometriesTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

# We run with the Network alg, which can generate node/way match pairs. Then we check to make sure tags changes based on node/way match pairs
# don't exist in the tags changeset. Particularly, you should not see any highway nodes in the output with the following tags transferred to
# them from ways: access=permissive, highway=service, and surface=asphalt.

# Run changeset w/tags to produce separate outputs for geometry and tags
echo "Running diff changeset with tags, separate outputs..."
hoot conflate $LOG_LEVEL -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 $INPUT_DIR/Input1.osm $INPUT_DIR/Input2.osm \
 $OUTPUT_DIR/output.osc --differential --include-tags --separate-output

# Check tag output
echo "Checking tag diff"
if diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc >/dev/null ; then
  echo "Tag Changeset Files Match"
else
  echo "Tag Changeset Files Don't Match"
  diff $OUTPUT_DIR/output.tags.osc $INPUT_DIR/output.tags.osc
fi
