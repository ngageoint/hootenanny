#!/bin/bash

INPATH=test-files/cmd/quick/AddMeasurementTagsTest
INPUTFILE=$INPATH/AddMeasurementTagsTestIn.osm
COMPAREFILE=$INPATH/AddMeasurementTagsTestOut.osm
OUTPUTFILE=test-output/cmd/quick/AddMeasurementTagsTestOut.osm

mkdir -p test-output/cmd/quick/
hoot convert -C Testing.conf -D convert.ops="hoot::AddMeasurementTagsVisitor" $INPUTFILE $OUTPUTFILE
hoot diff $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE
