#!/bin/bash
set -e

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

source scripts/database/SetupOsmApiDB.sh force
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

rm -rf test-output/cmd/ServiceDeriveChangesetCmdSqlTest
mkdir -p test-output/cmd/ServiceDeriveChangesetCmdSqlTest

hoot derive-changeset -D changeset.user.id=1 test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset.osc.sql $OSM_API_DB_URL
diff test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset.osc.sql test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset.osc.sql

