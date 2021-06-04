#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/glacial/ScoreMatchesCmdTest
OUTPUT_DIR=test-output/cmd/glacial/ScoreMatchesCmdTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL="--warn"
CONFIG="-C Testing.conf -D uuid.helper.repeatable=true"

# These files are kind of lame. Maybe make better test files in the future. 
map1=test-files/ScoreMatchRef1.osm
map2=test-files/ScoreMatchRef2.osm

# Run the command. Note that we sanitize output to remove timestamps.
echo "Running optimized..."
echo
hoot score-matches $LOG_LEVEL $CONFIG --confusion --optimize $map1 $map2 

# The optimize option doesn't write any output, so run again to check output. Tried running this with
# writer.include.conflate.score.tags=true, but get the error "Conflated uuid was not found in either input", which is worth
# looking into.
echo "Running unoptimized with output..."
echo
hoot score-matches $LOG_LEVEL $CONFIG $map1 $map2 $OUTPUT_DIR/output-1.osm
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/output-1.osm $INPUT_DIR/output-1.osm || diff $OUTPUT_DIR/output-1.osm $INPUT_DIR/output-1.osm

# Run with multiple inputs. Its a little silly to run with the same inputs twice, but we're mostly checking to see that
# the file handling logic works for now. Eventually, differing inputs should be selected to make the test more robust.
echo "Running optimized with multiple inputs..."
echo
hoot score-matches $LOG_LEVEL $CONFIG --confusion --optimize $map1 $map2 $map1 $map2

# Same as previous but running unoptimized.
echo "Running unoptimized with multiple inputs and an output..."
echo
hoot score-matches $LOG_LEVEL $CONFIG $map1 $map2 $map1 $map2 $OUTPUT_DIR/output-2.osm
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/output-2.osm $INPUT_DIR/output-2.osm || diff $OUTPUT_DIR/output-2.osm $INPUT_DIR/output-2.osm
