#!/bin/bash

# Loads data from a Hootenanny compatible data file into an OSM API database.  It assumes
# the database has already been initialized with a user.  If you do not specify starting
# changeset/element ID's, then the configuration option defaults will be used.  If you
# need to write subsequent datasets to the same database and don't want to pass in the
# starting ID's, use LoadOsmApiDbDataAutoCalcIds.sh.

set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

DATASET_TO_LOAD=$1
STARTING_CHANGESET_ID=$2
STARTING_NODE_ID=$3
STARTING_WAY_ID=$4
STARTING_RELATION_ID=$5
OUTPUT_SQL_SCRIPT=$HOOT_HOME/tmp/tmp.sql

if [ "$STARTING_CHANGESET_ID" == "" ]; then
  hoot convert $DATASET_TO_LOAD $OUTPUT_SQL_SCRIPT  
else
  hoot convert -D postgresql.dumpfile.writer.start.id.changeset=$STARTING_CHANGESET_ID -D postgresql.dumpfile.writer.start.id.node=$STARTING_NODE_ID -D postgresql.dumpfile.writer.start.id.way=$STARTING_WAY_ID -D postgresql.dumpfile.writer.start.id.relation=$STARTING_RELATION_ID $DATASET_TO_LOAD $OUTPUT_SQL_SCRIPT
fi
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f $OUTPUT_SQL_SCRIPT

