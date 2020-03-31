#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/JosmCleanCmdTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

CONFIG="-C Testing.conf"

inputfile=test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm
comparefile=test-files/ops/JosmMapCleanerTest/runCleanNoDetailTagsTest-out.osm

outputfile1=$OUTPUT_DIR/out1.osm
hoot clean $LOG_LEVEL $CONFIG -D map.cleaner.transforms="hoot::JosmMapCleaner" -D josm.validators.include="UntaggedWay;UnclosedWays;DuplicatedWayNodes" $inputfile $outputfile1
hoot diff $LOG_LEVEL $CONFIG $comparefile $outputfile1 || diff $comparefile $outputfile1

outputfile2=$OUTPUT_DIR/out2.osm
hoot convert $LOG_LEVEL $CONFIG -D convert.ops="hoot::JosmMapCleaner" -D josm.validators.include="UntaggedWay;UnclosedWays;DuplicatedWayNodes" $inputfile $outputfile2
hoot diff $LOG_LEVEL $CONFIG $comparefile $outputfile2 || diff $comparefile $outputfile2
