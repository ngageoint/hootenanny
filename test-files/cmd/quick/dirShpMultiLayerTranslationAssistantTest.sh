#!/bin/bash
set -e

export TA_FILE=calizip
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/translation_assistant_convert-ogr2osm
mkdir -p $TA_OUT

hoot convert --warn $TA_IN/$TA_FILE $TA_OUT/$TA_FILE.osm --trans $TA_IN/$TA_FILE-translation.js
hoot map-diff --ignore-uuid $TA_IN/$TA_FILE.osm $TA_OUT/$TA_FILE.osm 

