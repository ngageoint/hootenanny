#!/bin/bash

# Loads data from a Hootenanny compatible data file into an OSM API database.  It assumes
# the database has already been initialized with a user (see CleanAndInitializeOsmApiDb.sh).  
# The write operation will automatically determine the changeset/element starting ID's and 
# do so at the beginning of the write operation only.  

# THIS SCRIPT SHOULD NOT BE USED IN AN ENVIRONMENT WHERE MULTIPLE SIMULTANEOUS DATABASE 
# WRITERS EXIST, AS ID CONFLICTS MAY OCCUR.  It should only be used when the user of 
# this script is the only potential database writer.

set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

DATASET_TO_LOAD=$1
OUTPUT_SQL_SCRIPT=$HOOT_HOME/tmp/tmp.sql

echo "Creating SQL dump file from input data..."
hoot convert -D postgresql.dumpfile.writer.auto.calc.ids=true -D postgresql.dumpfile.writer.id.aware.url=$OSM_API_DB_URL $DATASET_TO_LOAD $OUTPUT_SQL_SCRIPT
echo "Executing SQL statements for data write..."
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f $OUTPUT_SQL_SCRIPT
echo "Data write completed."

