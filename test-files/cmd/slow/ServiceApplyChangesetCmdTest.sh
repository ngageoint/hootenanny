#!/bin/bash
set -e

# clean out the database
source scripts/SetupOsmApiDB.sh force
source conf/DatabaseConfig.sh
export DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql
#psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/changesets.sql

rm -rf test-output/cmd/ServiceApplyChangesetCmdTest
mkdir -p test-output/cmd/ServiceApplyChangesetCmdTest

# write the contents
hoot apply-changeset test-files/cmd/slow/ServiceApplyChangesetCmdTest/changeset.osc.sql $DB_URL

# read the contents back out
hoot convert $DB_URL test-output/cmd/ServiceApplyChangesetCmdTest/output.osm
hoot is-match test-files/cmd/slow/ServiceApplyChangesetCmdTest/output.osm test-output/cmd/ServiceApplyChangesetCmdTest/output.osm
