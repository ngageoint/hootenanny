
hoot convert -D osm.map.writer.factory.writer=hoot::OsmWriter -D convert.ops=hoot::TranslationOp -D translation.script=$HOOT_HOME/translations/GeoNames.js test-files/cmd/quick/ConvertGeoNames.geonames /dev/stdout
