#!/bin/bash

INPATH=test-files/cmd/quick/AddMeasurementTagsTest
INPUTFILE=$INPATH/AddMeasurementTagsTestIn.osm
COMPAREFILE=$INPATH/AddMeasurementTagsTestOut.osm
OUTPUTFILE=test-output/cmd/quick/AddMeasurementTagsTestOut.osm

CONFIG="-C Testing.conf -D writer.include.circular.error.tags=false"

mkdir -p test-output/cmd/quick/
hoot convert $CONFIG -D convert.ops="AddMeasurementTagsVisitor" $INPUTFILE $OUTPUTFILE
hoot diff $CONFIG $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE
