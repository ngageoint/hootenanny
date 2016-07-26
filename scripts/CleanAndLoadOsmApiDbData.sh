#!/bin/bash

# Completely clears an OSM API database, initializes it with a dummy user, and then loads data 
# from a Hootenanny compatible data file into it.  THIS SCRIPT WILL COMPLETELY OVERWRITE THE
# CONTENTS OF THE DATABASE WITH EACH EXECUTION.

set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

DATASET_TO_LOAD=$1
OUTPUT_SQL_SCRIPT=$HOOT_HOME/tmp/tmp.sql

source scripts/SetupOsmApiDB.sh force
# load dummy user
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

echo "Creating SQL dump file from input data..."
hoot convert $DATASET_TO_LOAD $OUTPUT_SQL_SCRIPT
echo "Executing SQL statements for data write..."
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f $OUTPUT_SQL_SCRIPT
echo "Data write completed."

