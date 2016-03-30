#!/bin/bash

source $HOOT_HOME/conf/DatabaseConfig.sh

# setup db, user, and password to avoid password prompt
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
export RENDER_DB="$DB_NAME"_renderdb_$INPUT
echo $RENDER_DB
#
#  Export hoot dataset to a PostGIS render db
#
createdb $AUTH $RENDER_DB -E UTF-8
if psql $AUTH -d $RENDER_DB -c 'CREATE EXTENSION postgis' &>/dev/null; then
    #Do Nothing
    :
else
    psql $AUTH -d $RENDER_DB -f /usr/share/postgresql/9.1/contrib/postgis-1.5/postgis.sql
    psql $AUTH -d $RENDER_DB -f /usr/share/postgresql/9.1/contrib/postgis-1.5/spatial_ref_sys.sql
fi

hoot osm2ogr -D services.db.reader.email=test@test.com -D services.db.writer.email=test@test.com $HOOT_HOME/translations/RenderDb.js "$DB_URL/$INPUT" "PG:dbname='$RENDER_DB' host='$DB_HOST' port='$DB_PORT' user='$DB_USER' password='$DB_PASSWORD'"
