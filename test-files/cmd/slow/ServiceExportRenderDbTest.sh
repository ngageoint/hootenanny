#!/bin/bash
set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh

export INPUT="Ex Ren Db"
# setup db, user, and password to avoid password prompt
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
export DB_URL=hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME
export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D api.db.email=test@test.com -D hootapi.db.writer.overwrite.map=true -D writer.include.debug=true --warn"

# Ingest the data
hoot --convert $HOOT_OPTS test-files/ExRenDb.osm "$DB_URL/$INPUT"

export MAP_ID=$(psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -qt -c "select id from maps where display_name = '$INPUT';" | tr -d ' ')
export RENDER_DB="$DB_NAME"_renderdb_"$MAP_ID"

# Preventative clean up
dropdb $AUTH $RENDER_DB &>/dev/null || true

# Write ingested db dataset to renderdb,
# to test the streaming reader to ogr writer
# with data that generated the error:
# Relation element did not exist in cache
scripts/services/exportrenderdb.sh $MAP_ID $DB_URL # || true # Don't error out so test will continue to clean up

# Clean up
hoot delete-map $HOOT_OPTS "$DB_URL/$INPUT"

# Assert that hoot map and render db have been deleted
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw "$INPUT" && echo "Error: delete-map did not remove $INPUT dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -lqt | cut -d \| -f 1 | grep -qw $RENDER_DB && echo "Error: delete-map did not remove $INPUT render db"
