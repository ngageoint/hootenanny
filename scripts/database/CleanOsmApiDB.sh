#!/bin/bash

# setup db, user, and password to avoid password prompt
source $HOOT_HOME/conf/database/DatabaseConfig.sh

export PGPASSWORD=$DB_PASSWORD_OSMAPI
export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"

# see if old db osmapi_test exists
export flag=`psql $AUTH -lqt | cut -d \| -f 1 | grep -w $DB_NAME_OSMAPI | wc -l`
if [ "$flag" = "1" ]; then
#  echo "Dropping old osm api db"
  dropdb $AUTH $DB_NAME_OSMAPI
fi
