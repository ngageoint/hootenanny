#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/StreamingApiDbTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
HOOT_OPTS="--warn -D uuid.helper.repeatable=true -D reader.add.source.datetime=false  -D writer.include.circular.error.tags=false -D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D changeset.user.id=1 -D osmapidb.bulk.inserter.reserve.record.ids.before.writing.data=false -D osmapidb.bulk.inserter.validate.data=true -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"

echo "streaming hoot api db --> osm api db..."
echo ""
hoot convert $HOOT_OPTS test-files/ToyTestA.osm "$HOOT_DB_URL/ToyTestA"
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OSM_API_DB_URL
hoot is-match $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OSM_API_DB_URL

echo "streaming osm api db --> hoot api db..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS test-files/ToyTestA.osm $OSM_API_DB_URL
hoot convert $HOOT_OPTS $OSM_API_DB_URL "$HOOT_DB_URL/ToyTestA"
hoot is-match $HOOT_OPTS $OSM_API_DB_URL "$HOOT_DB_URL/ToyTestA"

echo "streaming hoot api db --> pbf..."
echo ""
hoot convert $HOOT_OPTS test-files/ToyTestA.osm "$HOOT_DB_URL/ToyTestA"
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OUTPUT_DIR/ToyTestA-hootapidb.osm.pbf
hoot is-match $HOOT_OPTS "$HOOT_DB_URL/ToyTestA" $OUTPUT_DIR/ToyTestA-hootapidb.osm.pbf

echo "streaming osm api db --> pbf..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $HOOT_OPTS test-files/ToyTestA.osm $OSM_API_DB_URL
hoot convert $HOOT_OPTS $OSM_API_DB_URL $OUTPUT_DIR/ToyTestA-osmapidb.osm.pbf
hoot is-match $HOOT_OPTS $OSM_API_DB_URL $OUTPUT_DIR/ToyTestA-osmapidb.osm.pbf

