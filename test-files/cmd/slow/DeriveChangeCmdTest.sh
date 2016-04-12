#!/bin/bash
set -e

mkdir -p test-output/cmd/DeriveChangeCmdTest

hoot derive-change test-files/cmd/slow/DeriveChangeCmdTest/map1.osm test-files/cmd/slow/DeriveChangeCmdTest/map2.osm test-output/cmd/DeriveChangeCmdTest/changeset.osc
diff test-output/cmd/DeriveChangeCmdTest/changeset.osc test-files/cmd/slow/DeriveChangeCmdTest/changeset.osc 

source $HOOT_HOME/conf/DatabaseConfig.sh
hoot derive-change -D changeset.sql.use.internal.ids=true test-files/ToyTestA.osm test-files/cmd/slow/DeriveChangeCmdTest/conflate-out.osm test-output/cmd/DeriveChangeCmdTest/changeset.sql "postgresql://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
diff test-output/cmd/DeriveChangeCmdTest/changeset.sql test-files/cmd/slow/DeriveChangeCmdTest/changeset.sql

