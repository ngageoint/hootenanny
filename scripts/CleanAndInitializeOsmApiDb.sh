#!/bin/bash

# Completely clears an OSM API database and then loads data from a Hootenanny compatible data file 
# into it for testing purposes.  This script is meant to be used for one time dataset loading into 
# an OSM API database whose contents can be completely lost only.

set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD

source scripts/SetupOsmApiDB.sh force
# load dummy user
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

