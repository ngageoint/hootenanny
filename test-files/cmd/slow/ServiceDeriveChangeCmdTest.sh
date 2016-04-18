#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD

mkdir -p test-output/cmd/ServiceDeriveChangeCmdTest

hoot derive-change -D changeset.sql.use.internal.ids=true test-files/cmd/slow/DeriveChangeCmdTest/map1.osm test-files/cmd/slow/DeriveChangeCmdTest/map2.osm test-output/cmd/ServiceDeriveChangeCmdTest/changeset.osc.sql "$DB_URL"
diff test-output/cmd/ServiceDeriveChangeCmdTest/changeset.osc.sql test-files/cmd/slow/ServiceDeriveChangeCmdTest/changeset.osc.sql

