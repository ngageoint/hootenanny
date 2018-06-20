#!/bin/bash

# Do not 'set -e' because these tests are suppsed to fail

OUTPUT_DIR=$HOOT_HOME/test-output/cmd/quick

mkdir -p $OUTPUT_DIR

DATA_FILES=$HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot convert $DATA_FILES $OUTPUT_DIR/BadJavaScriptTranslation.osm --trans test-files/cmd/slow/tSimple.js

hoot convert $DATA_FILES $OUTPUT_DIR/BadJavaScriptTranslation.osm --trans plugins/BadSyntaxTest.js

hoot convert $DATA_FILES $OUTPUT_DIR/BadJavaScriptTranslation.osm --trans test-files/cmd/slow/BadRequire.js

