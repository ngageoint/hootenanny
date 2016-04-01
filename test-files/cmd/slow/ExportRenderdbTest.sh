#!/bin/bash
set -e
source $HOOT_HOME/conf/DatabaseConfig.sh

export INPUT=ExRenDb
# setup db, user, and password to avoid password prompt
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
export RENDER_DB="$DB_NAME"_renderdb_$INPUT
export DB_URL=postgresql://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME
export HOOT_OPTS="-D services.db.writer.create.user=true -D services.db.writer.email=test@test.com -D services.db.writer.overwrite.map=true -D services.db.reader.email=test@test.com -D writer.include.debug=true --warn"

# Preventative clean up
dropdb $AUTH $RENDER_DB &>/dev/null || true

# Ingest the data
hoot --convert $HOOT_OPTS test-files/$INPUT.osm $DB_URL/$INPUT

# Write ingested db dataset to renderdb,
# to test the streaming reader to ogr writer
# with data that generated the error:
# Relation element did not exist in cache
scripts/exportrenderdb.sh # || true # Don't error out so test will continue to clean up

# Clean up
dropdb $AUTH $RENDER_DB
hoot delete-map $HOOT_OPTS $DB_URL/$INPUT
