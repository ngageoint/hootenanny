#!/bin/bash


export DB_PASSWORD='vagrant'
export DB_USER='vagrant'
export DB_HOST='localhost'
export DB_PORT='15432'
export DB_NAME='openstreetmap'

echo "User="$DB_USER
echo "Password="$DB_PASSWORD
echo "DB Host="$DB_HOST
echo "DB_Name="$DB_NAME
 
# Drop all tables
SQL=$(psql -h $DB_HOST -p $DB_PORT -t -A -d $DB_NAME -U $DB_USER -c "select 'delete from \"' || tablename || '\" ;' from pg_tables where tableowner like '$DB_USER';")
echo "SQL = "$SQL
echo $SQL | psql -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER

# Drop all sequences
SQL=$(psql -h $DB_HOST -p $DB_PORT -t -A -d $DB_NAME -U $DB_USER -c "select 'alter SEQUENCE \"' || relname || '\" restart with 1;' FROM pg_class c WHERE c.relkind='S';")
echo "SEQ = "$SQL
echo $SQL | psql -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER
