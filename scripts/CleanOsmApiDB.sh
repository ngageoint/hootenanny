#!/bin/bash

# setup db, user, and password to avoid password prompt
export PGDATABASE=osmapi_test
export PGUSER=hoot
export PGPASSWORD=hoottest

# see if old db osmapi_test exists
export flag=`psql -lqt -U hoot | cut -d \| -f 1 | grep -w osmapi_test | wc -l`
if [ "$flag" = "1" ]; then
  echo "Dropping old osm api db"
  dropdb osmapi_test
fi
