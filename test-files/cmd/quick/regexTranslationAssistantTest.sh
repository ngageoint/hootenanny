#!/bin/bash
set -e

export TA_FILE=regex-translation
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/quick/regex-translation
mkdir -p $TA_OUT

hoot convert -C Testing.conf \
 -D writer.include.circular.error.tags=false \
 -D reader.add.source.datetime=false  \
 -D schema.translation.script=$TA_IN/${TA_FILE}.js \
 $TA_IN/$TA_FILE.shp \
 $TA_OUT/$TA_FILE.osm

hoot diff --ignore-uuid $TA_IN/$TA_FILE.osm $TA_OUT/$TA_FILE.osm 

