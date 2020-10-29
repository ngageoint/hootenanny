#!/bin/bash
set -e

source conf/database/DatabaseConfig.sh
export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export PSQL_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"

export HOOT_EMAIL="DbListMapsCmdTest@hoottestcpp.org"

export CONFIG="-C Testing.conf"

# add some maps for this user (delete any pre-existing)
hoot convert --warn $CONFIG -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true test-files/ToyTestA.osm $HOOT_DB_URL/DbListMapsCmdTest1
hoot convert --warn $CONFIG -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true test-files/ToyTestB.osm $HOOT_DB_URL/DbListMapsCmdTest2

# list the maps (not dealing with public maps here; see ServiceHootApiDbTest)
hoot db-list --warn $CONFIG -D api.db.email=$HOOT_EMAIL $HOOT_DB_URL

# clean-up the databases
hoot db-delete --warn $CONFIG -D api.db.email=$HOOT_EMAIL $HOOT_DB_URL/DbListMapsCmdTest1
hoot db-delete --warn $CONFIG -D api.db.email=$HOOT_EMAIL $HOOT_DB_URL/DbListMapsCmdTest2

# Delete the user
PGPASSWORD=$DB_PASSWORD psql $PSQL_DB_AUTH -c "DELETE FROM users WHERE email='$HOOT_EMAIL';" > /dev/null
