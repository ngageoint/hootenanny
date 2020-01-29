#!/bin/bash
set -e

export TA_FILE=calizip
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/quick/translation_assistant_convert-ogr2osm
mkdir -p $TA_OUT

hoot convert --warn -D duplicate.node.remover.distance.threshold=1.0 -D schema.translation.script=$TA_IN/$TA_FILE-translation.js $TA_IN/$TA_FILE $TA_OUT/$TA_FILE.osm
hoot diff --ignore-uuid $TA_IN/$TA_FILE.osm $TA_OUT/$TA_FILE.osm 

