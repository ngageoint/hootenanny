#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh

export HOOT_OPTS="-D services.db.writer.create.user=true -D services.db.writer.email=ServicesDbWriterTest@hoottestcpp.org -D services.db.writer.overwrite.map=true -D services.db.reader.email=ServicesDbWriterTest@hoottestcpp.org -D writer.include.debug=true --warn"

export DB_URL="postgresql://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

rm -rf test-output/cmd/ServiceDbTest
mkdir -p test-output/cmd/ServiceDbTest
hoot --delete-map $HOOT_OPTS "$DB_URL/AllDataTypesA" &
hoot --delete-map $HOOT_OPTS "$DB_URL/AllDataTypesB" &
wait
###
# I've run into strange situations where it seems like the whole files isn't being converted.
# It almost looks like the transaction isn't fully committed when the conflat op runs. I can't
# reproduce it reliably, but I would like to fix it if I can get more info. -JRS
# I think this is fixed by #3588, but can't really prove a negative. -JRS
##
hoot --convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesA.osm "$DB_URL/AllDataTypesA" &
hoot --convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesB.osm "$DB_URL/AllDataTypesB" &
wait
hoot --conflate $HOOT_OPTS "$DB_URL/AllDataTypesA" "$DB_URL/AllDataTypesB" test-output/cmd/ServiceDbTest/output.osm
# Do a very coarse comparison check.
hoot --stats --quick test-output/cmd/ServiceDbTest/output.osm

