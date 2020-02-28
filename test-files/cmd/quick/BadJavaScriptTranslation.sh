#!/bin/bash

# Do not 'set -e' because these tests are suppsed to fail

OUTPUT_DIR=$HOOT_HOME/test-output/cmd/quick

mkdir -p $OUTPUT_DIR

DATA_FILES=$HOOT_HOME/test-files/jakarta_raya_coastline.shp

CONFIG="-C Testing.conf"

hoot convert $CONFIG -D schema.translation.script=test-files/cmd/quick/tSimple.js $DATA_FILES $OUTPUT_DIR/BadJavaScriptTranslation.osm

hoot convert $CONFIG -D schema.translation.script=translations/BadSyntaxTest.js $DATA_FILES $OUTPUT_DIR/BadJavaScriptTranslation.osm

hoot convert $CONFIG -D schema.translation.script=test-files/cmd/quick/BadRequire.js $DATA_FILES $OUTPUT_DIR/BadJavaScriptTranslation.osm

