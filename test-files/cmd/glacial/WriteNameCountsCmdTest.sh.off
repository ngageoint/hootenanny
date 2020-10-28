#!/bin/bash
set -e

INPUT_DIR=test-files
OUTPUT_DIR=test-output/cmd/glacial/WriteNameCountsCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot write-name-counts --warn -C Testing.conf "$INPUT_DIR/DcGisRoads.osm;$INPUT_DIR/DcTigerRoads.osm" $OUTPUT_DIR/nameCounts.db
