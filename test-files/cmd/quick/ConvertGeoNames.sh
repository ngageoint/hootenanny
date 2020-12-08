#!/bin/bash
set -e

# GeoNames.org
hoot convert --warn -C Testing.conf -D map.factory.writer=hoot::OsmXmlWriter -D convert.ops=hoot::SchemaTranslationVisitor -D schema.translation.script=$HOOT_HOME/translations/GeoNames_to_OSM.js test-files/cmd/quick/ConvertGeoNames.geonames /dev/stdout

# geonames.nga.mil
hoot convert --warn -C Testing.conf -D map.factory.writer=hoot::OsmXmlWriter -D convert.ops=hoot::SchemaTranslationVisitor -D schema.translation.script=$HOOT_HOME/translations/GeoNames_to_OSM.js test-files/cmd/quick/ConvertGeoNamesMil.geonames /dev/stdout
