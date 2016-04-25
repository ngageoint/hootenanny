#!/bin/bash
set -e

# We want to check to see if the $DB_NAME_OSMAPI exists. If it does not exist,
# we create it. If it already exists, we check its creation date against its
# sql definition file. If the date on the file is newer than the date the db
# was created, we drop it & re-create. Otherwise we leave the DB alone. 

source $HOOT_HOME/conf/DatabaseConfig.sh

# setup db, user, and password to avoid password prompt
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
do_create="true"

# see if old db osmapi_test exists
export flag=`psql $AUTH -lqt | cut -d \| -f 1 | grep -w "^ $DB_NAME_OSMAPI \+" | wc -l`

if [ "$flag" = "1" ]; then
  # DB Exists, check creation date:
  SQL_SCRIPT="CREATE OR REPLACE FUNCTION checkDbTime() RETURNS text LANGUAGE plpgsql AS \$\$ 
            DECLARE db_oid text; 
                    data_dir text;  
                    file_name text;  
                    file_time text;  
            BEGIN  
              SELECT INTO db_oid oid FROM pg_database WHERE datname='osmapi_test'; 
              SELECT INTO data_dir setting FROM pg_settings WHERE name='data_directory';  
              file_name := data_dir || '/base/' || db_oid || '/PG_VERSION'; 
              SELECT INTO file_time modification FROM pg_stat_file(file_name); 
              RETURN '[' || file_time || ']';
            END 
            \$\$; 
            SELECT checkDbTime();"

  # exec SQL
  db_date_str=`echo $SQL_SCRIPT | psql $AUTH`

  # clean date string
  db_date_str=${db_date_str#*[}
  db_date_str=${db_date_str%]*}

  # get sql file timestamp
  file_date_str=`stat -c "%y" $HOOT_HOME/scripts/osmapidb/blank_osmapidb.sql`;

  # Convert strings to seconds for comparison purposes
  db_date=`date -d "$db_date_str" "+%s"`
  file_date=`date -d "$file_date_str" "+%s"`;

  if [ $file_date -ge $db_date ]; then
    # Drop the DB
    dropdb $AUTH $DB_NAME_OSMAPI
    do_create="true"
  else
    # DB exists, and appears to be up to date, do nothing
    do_create="false"
  fi
fi

# create the osm apu db from the blank osm api db script
if [ "$do_create" = "true" ]; then
  #echo "Creating osm api db"
  #echo "DB_HOST: " $DB_HOST
  #echo "DB_PORT: " $DB_PORT
  #echo "DB_USER: " $DB_USER
  #echo "PGPASSWORD: " $PGPASSWORD
  #echo "DB_NAME_OSMAPI: " $DB_NAME_OSMAPI
  createdb $AUTH $DB_NAME_OSMAPI

  mkdir -p $HOOT_HOME/tmp

  psql $AUTH -d $DB_NAME_OSMAPI -f $HOOT_HOME/scripts/osmapidb/blank_osmapidb.sql >& $HOOT_HOME/tmp/osmapidb.log
fi

