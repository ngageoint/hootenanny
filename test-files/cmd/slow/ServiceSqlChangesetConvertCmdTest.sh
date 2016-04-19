#!/bin/bash
set -e

# clean out the database
source scripts/SetupOsmApiDB.sh
# setup DB variables for automation
source conf/DatabaseConfig.sh
export DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/changesets.sql

rm -rf test-output/cmd/ServiceSqlChangesetConvertCmdTest
mkdir -p test-output/cmd/ServiceSqlChangesetConvertCmdTest

# write the contents
hoot convert test-files/cmd/slow/ServiceSqlChangesetConvertCmdTest/changeset.osc.sql $DB_URL

# read the contents back out
hoot convert $DB_URL test-output/cmd/ServiceSqlChangesetConvertCmdTest/output.osm
hoot is-match test-files/cmd/slow/ServiceSqlChangesetConvertCmdTest/output.osm test-output/cmd/ServiceSqlChangesetConvertCmdTest/output.osm
