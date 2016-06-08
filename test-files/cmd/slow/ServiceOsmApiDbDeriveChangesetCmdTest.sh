#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export DB_URL="osmapidb://$DB_USER_OSMAPI:$DB_PASSWORD_OSMAPI@$DB_HOST_OSMAPI:$DB_PORT_OSMAPI/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

mkdir -p test-output/cmd/ServiceOsmApiDbDeriveChangesetCmdTest

hoot derive-changeset test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceOsmApiDbDeriveChangesetCmdTest/changeset.osc.sql "$DB_URL"
diff test-output/cmd/ServiceOsmApiDbDeriveChangesetCmdTest/changeset.osc.sql test-files/cmd/slow/ServiceOsmApiDbDeriveChangesetCmdTest/changeset.osc.sql

