

DATA_FILES=$HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot ogr2osm test-files/cmd/slow/tSimple.js test-output/cmd/slow/BadJavaScriptTranslation.osm $DATA_FILES

hoot ogr2osm plugins/BadSyntaxTest.js test-output/cmd/slow/BadJavaScriptTranslation.osm $DATA_FILES

hoot ogr2osm test-files/cmd/slow/BadRequire.js test-output/cmd/slow/BadJavaScriptTranslation.osm $DATA_FILES

