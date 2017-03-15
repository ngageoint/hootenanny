#!/bin/bash

# Completely clears an OSM API database and initializes it with a dummy user.

set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

source $HOOT_HOME/scripts/database/SetupOsmApiDB.sh force
# load dummy user
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f $HOOT_HOME/test-files/servicesdb/users.sql
