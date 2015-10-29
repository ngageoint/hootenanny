#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh

# setup db, user, and password to avoid password prompt
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD

# see if old db osmapi_test exists
export flag=`psql $AUTH -lqt | cut -d \| -f 1 | grep -w $DB_NAME_OSMAPI | wc -l`
if [ "$flag" = "1" ]; then
#  echo "Dropping old osm api db"
  dropdb $AUTH $DB_NAME_OSMAPI
fi

# create the osm apu db from the blank osm api db script
#echo "Creating osm api db"
createdb $AUTH $DB_NAME_OSMAPI
psql $AUTH -d $DB_NAME_OSMAPI -f $HOOT_HOME/scripts/osmapidb/blank_osmapidb.sql >& /tmp/osmapidb.log
