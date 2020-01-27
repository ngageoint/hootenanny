#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/quick/CleanCmdJosmTest
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

inputfile=test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm
comparefile=test-files/ops/JosmMapCleanerTest/runCleanNoDetailTagsTest-out.osm

outputfile1=$OUTPUT_DIR/out1.osm
hoot clean $LOG_LEVEL -C Testing.conf -D map.cleaner.transforms="hoot::JosmMapCleaner" -D josm.validators.include="UntaggedWay;UnclosedWays;DuplicatedWayNodes" $inputfile $outputfile1
hoot diff $LOG_LEVEL $comparefile $outputfile1 || diff $comparefile $outputfile1

outputfile2=$OUTPUT_DIR/out2.osm
hoot convert $LOG_LEVEL -C Testing.conf -D convert.ops="hoot::JosmMapCleaner" -D josm.validators.include="UntaggedWay;UnclosedWays;DuplicatedWayNodes" $inputfile $outputfile2
hoot diff $LOG_LEVEL $comparefile $outputfile2 || diff $comparefile $outputfile2
