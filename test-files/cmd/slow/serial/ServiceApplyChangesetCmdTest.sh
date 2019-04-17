#!/bin/bash
set -e

# clean out the database
source scripts/database/SetupOsmApiDB.sh force
source conf/database/DatabaseConfig.sh
export DB_URL="osmapidb://$DB_USER_OSMAPI:$DB_PASSWORD_OSMAPI@$DB_HOST_OSMAPI:$DB_PORT_OSMAPI/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

rm -rf test-output/cmd/slow/serial/ServiceApplyChangesetCmdTest
mkdir -p test-output/cmd/slow/serial/ServiceApplyChangesetCmdTest

# write the contents
hoot changeset-apply --debug test-files/cmd/slow/serial/ServiceApplyChangesetCmdTest/changeset.osc.sql $DB_URL

# read the contents back out
hoot convert $DB_URL test-output/cmd/slow/serial/ServiceApplyChangesetCmdTest/output.osm
hoot diff test-files/cmd/slow/serial/ServiceApplyChangesetCmdTest/output.osm test-output/cmd/slow/serial/ServiceApplyChangesetCmdTest/output.osm
