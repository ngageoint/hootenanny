#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ChangesetDeriveCmdTest
INPUT_DIR_2=test-files/cmd/slow/serial/ServiceChangesetDeriveCmdSqlTest
OUTPUT_DIR=test-output/cmd/slow/serial/ServiceChangesetDeriveCmdSqlTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
LOG_LEVEL=--warn

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

source scripts/database/SetupOsmApiDB.sh force
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

# The output between these two sets will be identical except for the auto-incrementing of element IDs that occurs by the db. So
# for that reason, we compare against two sets of gold files where the only difference between them are the incremented IDs.

# in memory sorting tests

hoot changeset-derive $LOG_LEVEL -C Testing.conf -D changeset.user.id=1 $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-1.osc.sql $OSM_API_DB_URL
diff $INPUT_DIR_2/changeset-1.osc.sql $OUTPUT_DIR/changeset-1.osc.sql 

hoot changeset-derive $LOG_LEVEL -C Testing.conf -D changeset.user.id=1 $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-2.osc.sql $OSM_API_DB_URL
diff $INPUT_DIR_2/changeset-2.osc.sql $OUTPUT_DIR/changeset-2.osc.sql 

# external merge sorting tests

hoot changeset-derive $LOG_LEVEL -C Testing.conf -D changeset.user.id=1 -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm $INPUT_DIR/map2.osm $OUTPUT_DIR/changeset-3.osc.sql $OSM_API_DB_URL
diff $INPUT_DIR_2/changeset-3.osc.sql $OUTPUT_DIR/changeset-3.osc.sql 

hoot changeset-derive $LOG_LEVEL -C Testing.conf -D changeset.user.id=1 -D element.sorter.element.buffer.size=5 $INPUT_DIR/map1.osm "" $OUTPUT_DIR/changeset-4.osc.sql $OSM_API_DB_URL
diff $INPUT_DIR_2/changeset-4.osc.sql $OUTPUT_DIR/changeset-4.osc.sql 

scripts/database/CleanAndInitializeOsmApiDb.sh
