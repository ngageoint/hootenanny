#!/bin/bash
set -e

EMAIL=DbListMapsCmdTest@hoottestcpp.org
source conf/database/DatabaseConfig.sh
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

CONFIG="-C Testing.conf"

# add some maps for this user (delete any pre-existing)
hoot convert --warn $CONFIG -D api.db.email=$EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true test-files/ToyTestA.osm $HOOT_DB_URL/DbListMapsCmdTest1
hoot convert --warn $CONFIG -D api.db.email=$EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true test-files/ToyTestB.osm $HOOT_DB_URL/DbListMapsCmdTest2

# list the maps (not dealing with public maps here; see ServiceHootApiDbTest)
hoot db-list-maps --warn $CONFIG -D api.db.email=$EMAIL $HOOT_DB_URL

# clean-up the databases
hoot db-delete-map --warn $CONFIG -D api.db.email=$EMAIL $HOOT_DB_URL/DbListMapsCmdTest1
hoot db-delete-map --warn $CONFIG -D api.db.email=$EMAIL $HOOT_DB_URL/DbListMapsCmdTest2
