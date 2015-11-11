#!/bin/bash

# setup db, user, and password to avoid password prompt
source $HOOT_HOME/conf/DatabaseConfig.sh

export PGPASSWORD=$DB_PASSWORD
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"

# see if old db osmapi_test exists
export flag=`psql $AUTH -lqt | cut -d \| -f 1 | grep -w $DB_NAME_OSMAPI | wc -l`
if [ "$flag" = "1" ]; then
#  echo "Dropping old osm api db"
  dropdb $AUTH $DB_NAME_OSMAPI
fi
