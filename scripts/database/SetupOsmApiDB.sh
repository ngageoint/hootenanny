#!/bin/bash
set -e

# We want to check to see if the $DB_NAME_OSMAPI exists. If it does not exist,
# we create it. If it already exists, we check its creation date against its
# sql definition file. If the date on the file is newer than the date the db
# was created, we drop it & re-create. Otherwise we leave the DB alone. 

source $HOOT_HOME/conf/database/DatabaseConfig.sh

# setup db, user, and password to avoid password prompt
export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
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
              SELECT INTO db_oid oid FROM pg_database WHERE datname='$DB_NAME_OSMAPI'; 
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
  file_date_str=`stat -c "%y" $HOOT_HOME/scripts/database/blank_osmapidb.sql`;

  # Convert strings to seconds for comparison purposes
  db_date=`date -d "$db_date_str" "+%s"`
  file_date=`date -d "$file_date_str" "+%s"`;

  if [ $file_date -ge $db_date ] || [ "$1" = "force" ]; then
    # Drop the DB
    # echo "DROPPING $DB_NAME_OSMAPI"
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
  #echo "DB_HOST_OSMAPI: " $DB_HOST_OSMAPI
  #echo "DB_PORT_OSMAPI: " $DB_PORT_OSMAPI
  #echo "DB_USER_OSMAPI: " $DB_USER_OSMAPI
  #echo "PGPASSWORD: " $PGPASSWORD
  #echo "DB_NAME_OSMAPI: " $DB_NAME_OSMAPI
  createdb $AUTH $DB_NAME_OSMAPI

  psql $AUTH -d $DB_NAME_OSMAPI -f $HOOT_HOME/scripts/database/blank_osmapidb.sql >& /tmp/osmapidb.log
fi

