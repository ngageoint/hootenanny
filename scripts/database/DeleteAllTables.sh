#!/bin/bash

source $HOOT_HOME/conf/database/DatabaseConfig.sh

export PGPASSWORD=$DB_PASSWORD

# Drop all tables
SQL=$(psql -h $DB_HOST -t -A -p $DB_PORT -d $DB_NAME -U $DB_USER -c "select 'drop table \"' || tablename || '\" cascade;' from pg_tables where tableowner like '$DB_USER';")
echo $SQL | psql -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER

# Drop all sequences
SQL=$(psql -h $DB_HOST -t -A -p $DB_PORT -d $DB_NAME -U $DB_USER -c "SELECT 'DROP SEQUENCE \"' || relname || '\";' FROM pg_class c WHERE c.relkind='S';")
echo $SQL | psql -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER

# Drop all render dbs
SQL=$( psql -h $DB_HOST -t -A -p $DB_PORT -d $DB_NAME -U $DB_USER -c "SELECT 'DROP DATABASE \"' || datname || '\";' FROM pg_database WHERE datname like '"$DB_NAME"_renderdb\_%';" )
echo $SQL | psql -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER
