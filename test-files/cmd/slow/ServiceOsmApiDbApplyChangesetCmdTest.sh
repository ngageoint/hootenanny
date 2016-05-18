#!/bin/bash
set -e

# clean out the database
source scripts/SetupOsmApiDB.sh force
source conf/DatabaseConfig.sh
export DB_URL="osmapidb://$DB_USER_OSMAPI:$DB_PASSWORD_OSMAPI@$DB_HOST_OSMAPI:$DB_PORT_OSMAPI/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

rm -rf test-output/cmd/ServiceOsmApiDbApplyChangesetCmdTest
mkdir -p test-output/cmd/ServiceOsmApiDbApplyChangesetCmdTest

# write the contents
hoot apply-changeset test-files/cmd/slow/ServiceOsmApiDbApplyChangesetCmdTest/changeset.osc.sql $DB_URL

# read the contents back out
hoot convert $DB_URL test-output/cmd/ServiceOsmApiDbApplyChangesetCmdTest/output.osm
hoot is-match test-files/cmd/slow/ServiceOsmApiDbApplyChangesetCmdTest/output.osm test-output/cmd/ServiceOsmApiDbApplyChangesetCmdTest/output.osm
