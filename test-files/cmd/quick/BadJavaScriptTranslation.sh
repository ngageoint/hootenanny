#!/bin/bash

# Do not 'set -e' because these tests are suppsed to fail

OUTPUT_DIR=$HOOT_HOME/test-output/cmd/quick

mkdir -p $OUTPUT_DIR

DATA_FILES=$HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot convert-ogr2osm test-files/cmd/slow/tSimple.js $OUTPUT_DIR/BadJavaScriptTranslation.osm $DATA_FILES

hoot convert-ogr2osm plugins/BadSyntaxTest.js $OUTPUT_DIR/BadJavaScriptTranslation.osm $DATA_FILES

hoot convert-ogr2osm test-files/cmd/slow/BadRequire.js $OUTPUT_DIR/BadJavaScriptTranslation.osm $DATA_FILES

