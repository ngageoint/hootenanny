#!/bin/bash
set -e

GOLD_DIR=test-files/cmd/glacial/serial/ServiceStreamingApiDbTest
OUTPUT_DIR=test-output/cmd/glacial/serial/ServiceStreamingApiDbTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export PSQL_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
export HOOT_EMAIL="ServiceStreamingApiDbTest@hoottestcpp.org"
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
HOOT_OPTS="--warn -C Testing.conf -D uuid.helper.repeatable=true -D reader.add.source.datetime=false  -D writer.include.circular.error.tags=false -D changeset.user.id=1 -D osmapidb.bulk.inserter.reserve.record.ids.before.writing.data=false -D apidb.bulk.inserter.validate.data=true -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D api.db.email=$HOOT_EMAIL"

echo "streaming hoot api db --> osm api db..."
echo ""
hoot convert $HOOT_OPTS -D hootapi.db.writer.remap.ids=true test-files/ToyTestA.osm "$HOOT_DB_URL/ToyTestA"
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OSM_API_DB_URL
hoot diff $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OSM_API_DB_URL

echo "streaming osm api db --> hoot api db..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS test-files/ToyTestA.osm $OSM_API_DB_URL
hoot convert $HOOT_OPTS $OSM_API_DB_URL "$HOOT_DB_URL/ToyTestA"
hoot diff $HOOT_OPTS $OSM_API_DB_URL "$HOOT_DB_URL/ToyTestA"

echo "streaming hoot api db --> pbf..."
echo ""
hoot convert $HOOT_OPTS -D hootapi.db.writer.remap.ids=true test-files/ToyTestA.osm "$HOOT_DB_URL/ToyTestA"
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OUTPUT_DIR/ToyTestA-hootapidb.osm.pbf
hoot diff $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OUTPUT_DIR/ToyTestA-hootapidb.osm.pbf

echo "streaming osm api db --> pbf..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS test-files/ToyTestA.osm $OSM_API_DB_URL
hoot convert $HOOT_OPTS $OSM_API_DB_URL $OUTPUT_DIR/ToyTestA-osmapidb.osm.pbf
hoot diff $HOOT_OPTS $OSM_API_DB_URL $OUTPUT_DIR/ToyTestA-osmapidb.osm.pbf

echo "streaming hoot api db --> xml..."
echo ""
hoot convert $HOOT_OPTS -D hootapi.db.writer.remap.ids=true test-files/ToyTestA.osm "$HOOT_DB_URL/ToyTestA"
hoot convert $HOOT_OPTS -D writer.xml.sort.by.id=false "$HOOT_DB_URL/ToyTestA" $OUTPUT_DIR/ToyTestA-hootapidb.osm
hoot diff $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OUTPUT_DIR/ToyTestA-hootapidb.osm

echo "streaming osm api db --> xml..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS test-files/ToyTestA.osm $OSM_API_DB_URL
hoot convert $HOOT_OPTS -D writer.xml.sort.by.id=false $OSM_API_DB_URL $OUTPUT_DIR/ToyTestA-osmapidb.osm
hoot diff $HOOT_OPTS $OSM_API_DB_URL $OUTPUT_DIR/ToyTestA-osmapidb.osm

# need to make sure streaming I/O is *not* done if a bounds is specified, since the bound reading doesn't support streaming

BOUNDS="-104.7223166,38.8845025,-104.7148732,38.8975378"

echo "hoot api db bounds --> xml"
echo ""
hoot convert $HOOT_OPTS -D hootapi.db.writer.remap.ids=true test-files/conflate/point/Poi1.osm "$HOOT_DB_URL/Poi1"
hoot convert $HOOT_OPTS -D bounds=$BOUNDS -D writer.xml.sort.by.id=false "$HOOT_DB_URL/Poi1" $OUTPUT_DIR/Poi1-cropped-hootapidb.osm
hoot diff $HOOT_OPTS $GOLD_DIR/Poi1-cropped.osm $OUTPUT_DIR/Poi1-cropped-hootapidb.osm

echo "osm api db bounds --> xml"
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS test-files/conflate/point/Poi1.osm $OSM_API_DB_URL
hoot convert $HOOT_OPTS -D bounds=$BOUNDS -D writer.xml.sort.by.id=false $OSM_API_DB_URL $OUTPUT_DIR/Poi1-cropped-osmapidb.osm
hoot diff $HOOT_OPTS $GOLD_DIR/Poi1-cropped.osm $OUTPUT_DIR/Poi1-cropped-osmapidb.osm

echo "hoot api db bounds --> ogr"
echo ""
hoot convert $HOOT_OPTS -D hootapi.db.writer.remap.ids=true test-files/conflate/point/Poi1.osm "$HOOT_DB_URL/Poi1"
hoot convert $HOOT_OPTS -D schema.translation.script=translations/Poi.js -D bounds=$BOUNDS "$HOOT_DB_URL/Poi1" $OUTPUT_DIR/Poi1-cropped-hootapidb.shp
hoot convert $HOOT_OPTS $OUTPUT_DIR/Poi1-cropped-hootapidb/poi.shp $OUTPUT_DIR/Poi1-cropped-hootapidb-ogr.osm
hoot diff $HOOT_OPTS $GOLD_DIR/Poi1-cropped-2.osm $OUTPUT_DIR/Poi1-cropped-hootapidb-ogr.osm

echo "osm api db bounds --> ogr"
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS test-files/conflate/point/Poi1.osm $OSM_API_DB_URL
hoot convert $HOOT_OPTS -D schema.translation.script=translations/Poi.js -D bounds=$BOUNDS $OSM_API_DB_URL $OUTPUT_DIR/Poi1-cropped-osmapidb.shp
hoot convert $HOOT_OPTS $OUTPUT_DIR/Poi1-cropped-osmapidb/poi.shp $OUTPUT_DIR/Poi1-cropped-osmapidb-ogr.osm
hoot diff $HOOT_OPTS $GOLD_DIR/Poi1-cropped-2.osm $OUTPUT_DIR/Poi1-cropped-osmapidb-ogr.osm

# Cleanup the database
hoot db-delete -D api.db.email=$HOOT_EMAIL "$HOOT_DB_URL/ToyTestA" "$HOOT_DB_URL/Poi1"
scripts/database/CleanAndInitializeOsmApiDb.sh

# Delete the user
PGPASSWORD=$DB_PASSWORD psql $PSQL_DB_AUTH -d $DB_NAME -c "DELETE FROM users WHERE email='$HOOT_EMAIL';" > /dev/null
