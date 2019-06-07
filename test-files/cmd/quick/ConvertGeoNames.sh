#!/bin/bash
set -e

hoot convert -C Testing.conf -D osm.map.writer.factory.writer=hoot::OsmXmlWriter -D convert.ops=hoot::TranslationOp -D schema.translation.script=$HOOT_HOME/translations/GeoNames.js test-files/cmd/quick/ConvertGeoNames.geonames /dev/stdout
