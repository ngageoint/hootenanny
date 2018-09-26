#!/bin/bash
set -e

rm -rf test-output/cmd/ServiceDeriveChangesetCmdSqlTest
mkdir -p test-output/cmd/ServiceDeriveChangesetCmdSqlTest

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

source scripts/database/SetupOsmApiDB.sh force
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

# The output between these two sets will be identical except for the auto-incrementing of element IDs that occurs by the db.  So
# for that reason, we compare against two sets of gold files where the only difference between them are the incremented IDs.

# in memory sorting tests

hoot changeset-derive -D changeset.user.id=1 test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-1.osc.sql $OSM_API_DB_URL
diff test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-1.osc.sql test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-1.osc.sql 

hoot changeset-derive -D changeset.user.id=1 test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-2.osc.sql $OSM_API_DB_URL
diff test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-2.osc.sql test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-2.osc.sql 

# external merge sorting tests

hoot changeset-derive -D changeset.user.id=1 -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=osm test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc.sql $OSM_API_DB_URL
diff test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc.sql test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-3.osc.sql 

hoot changeset-derive -D changeset.user.id=1 -D element.sorter.element.buffer.size=5 -D element.sorter.external.temp.format=osm test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm "" test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-4.osc.sql $OSM_API_DB_URL
diff test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset-4.osc.sql test-output/cmd/ServiceDeriveChangesetCmdSqlTest/changeset-4.osc.sql 

scripts/database/CleanAndInitializeOsmApiDb.sh
