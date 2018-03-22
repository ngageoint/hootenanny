#!/bin/bash
set -e

export TA_FILE=calizip
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/translation_assistant_ogr2osm
mkdir -p $TA_OUT

hoot ogr2osm $TA_IN/$TA_FILE-translation.js $TA_OUT/$TA_FILE.osm $TA_IN/$TA_FILE
hoot map-is-match --ignore-uuid $TA_OUT/$TA_FILE.osm $TA_IN/$TA_FILE.osm

