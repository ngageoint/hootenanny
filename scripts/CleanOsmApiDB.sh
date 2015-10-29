#!/bin/bash

# setup db, user, and password to avoid password prompt
source $HOOT_HOME/conf/DatabaseConfig.sh

export AUTH="-h $PGHOST -p $PGPORT -U $PGUSER"

# see if old db osmapi_test exists
export flag=`psql $AUTH -lqt | cut -d \| -f 1 | grep -w $PGDATABASE | wc -l`
if [ "$flag" = "1" ]; then
#  echo "Dropping old osm api db"
  dropdb $AUTH $PGDATABASE
fi
