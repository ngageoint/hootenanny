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
outputfile=$OUTPUT_DIR/output.osm

# Run the command. Note that we sanitize output to remove timestamps
echo "Running optimized..."
echo
hoot score-matches $LOG_LEVEL $CONFIG --confusion --optimize $map1 $map2 $outputfile 
# The optimize option doesn't write any output (#4649), so run again to check output. Tried running this with
# writer.include.conflate.score.tags=true, but get the error "Conflated uuid was not found in either input", which is worth
# looking into.
echo "Running unoptimized with output..."
echo
hoot score-matches $LOG_LEVEL $CONFIG $map1 $map2 $outputfile
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm || diff $OUTPUT_DIR/output.osm $INPUT_DIR/output.osm
