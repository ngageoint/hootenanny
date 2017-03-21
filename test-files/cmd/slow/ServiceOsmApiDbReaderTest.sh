#!/bin/bash
set -e

#echo $PGDATABASE $PGHOST $PGPORT $PGUSER $PGPASSWORD

# clean out the database
source scripts/database/SetupOsmApiDB.sh force

# setup DB variables for automation
source conf/database/DatabaseConfig.sh

export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

# setup dirs
OUTPUT_DIR=test-output/cmd/slow/ServiceOsmApiDbReaderTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

export HOOT_OPTS="-D reader.add.source.datetime=false -D uuid.helper.repeatable=true -D reader.preserve.all.tags=true -D writer.include.circular.error=false -D changeset.user.id=1 -D osmapidb.bulk.writer.reserve.record.ids.before.writing.data=true -D osmapidb.bulk.writer.output.files.copy.location=$OUTPUT_DIR/DcGisRoads.sql"
export DB_URL="osmapidb://$DB_USER_OSMAPI:$DB_PASSWORD_OSMAPI@$DB_HOST_OSMAPI:$DB_PORT_OSMAPI/$DB_NAME_OSMAPI"

echo "Select all from osm api db..."

psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/ToyTestA.sql

hoot convert --error $HOOT_OPTS "$DB_URL" $OUTPUT_DIR/output1.osm
hoot is-match test-files/cmd/slow/ServiceOsmApiDbReaderTest/output1.osm $OUTPUT_DIR/output1.osm

echo "Bounding box selection from osm api db..."

export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

scripts/database/CleanAndInitializeOsmApiDb.sh

hoot convert --error $HOOT_OPTS test-files/DcGisRoads.osm $DB_URL

hoot convert --error $HOOT_OPTS -D convert.bounding.box=-77.04,38.8916,-77.03324,38.8958 $OSM_API_DB_URL $OUTPUT_DIR/output2.osm
hoot is-match test-files/cmd/slow/ServiceOsmApiDbReaderTest/output2.osm $OUTPUT_DIR/output2.osm


