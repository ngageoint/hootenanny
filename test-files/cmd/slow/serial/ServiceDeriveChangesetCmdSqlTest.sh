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

hoot changeset-derive -D changeset.user.id=1 test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-1.osc.sql $OSM_API_DB_URL
diff test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-1.osc.sql test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-1.osc.sql

hoot changeset-derive -D changeset.user.id=1 test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-2.osc.sql $OSM_API_DB_URL
diff test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-2.osc.sql test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-2.osc.sql

hoot changeset-derive --debug -D changeset.user.id=1 test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm "test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc.sql;test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc" $OSM_API_DB_URL
diff test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc.sql test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc.sql
diff test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc

scripts/database/CleanAndInitializeOsmApiDb.sh
