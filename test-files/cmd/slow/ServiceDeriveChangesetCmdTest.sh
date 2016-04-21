#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD

mkdir -p test-output/cmd/ServiceDeriveChangesetCmdTest

hoot derive-changeset -D changeset.user.id=1 -D changeset.sql.use.internal.ids=true test-files/cmd/slow/DeriveChangesetCmdTest/map1.osm test-files/cmd/slow/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceDeriveChangesetCmdTest/changeset.osc.sql "$DB_URL"
diff test-output/cmd/ServiceDeriveChangesetCmdTest/changeset.osc.sql test-files/cmd/slow/ServiceDeriveChangesetCmdTest/changeset.osc.sql

