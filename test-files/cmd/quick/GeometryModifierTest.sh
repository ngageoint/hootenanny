#!/bin/bash

INPATH=test-files/cmd/quick/GeoModifierTest
RULESFILE=$INPATH/GeoModifierTestRules.json
INPUTFILE=$INPATH/GeoModifierTestIn.osm
COMPAREFILE=$INPATH/GeoModifierTestOut.osm
OUTPUTFILE=test-output/cmd/quick/GeoModifierTestOut.osm

mkdir -p test-output/cmd/quick/
hoot convert -C Testing.conf -D convert.ops="hoot::GeometryModifierOp" -D geometry.modifier.rules.file=$RULESFILE $INPUTFILE $OUTPUTFILE
hoot diff $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE
