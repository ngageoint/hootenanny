#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh

export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D hootapi.db.writer.email=ServiceHootApiDbConflateTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D hootapi.db.reader.email=ServiceHootApiDbConflateTest@hoottestcpp.org -D writer.include.debug=true --warn"

export DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

rm -rf test-output/cmd/ServiceHootApiDbConflateTest
mkdir -p test-output/cmd/ServiceHootApiDbConflateTest
hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesA" &
hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesB" &
wait
###
# I've run into strange situations where it seems like the whole files isn't being converted.
# It almost looks like the transaction isn't fully committed when the conflat op runs. I can't
# reproduce it reliably, but I would like to fix it if I can get more info. -JRS
# I think this is fixed by #3588, but can't really prove a negative. -JRS
##
hoot convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesA.osm "$DB_URL/AllDataTypesA" &
hoot convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesB.osm "$DB_URL/AllDataTypesB" &
wait
hoot conflate $HOOT_OPTS "$DB_URL/AllDataTypesA" "$DB_URL/AllDataTypesB" test-output/cmd/ServiceHootApiDbConflateTest/output.osm
# Do a very coarse comparison check.
hoot stats --quick test-output/cmd/ServiceHootApiDbConflateTest/output.osm

hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesA" &
hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesB" &
wait

export PGPASSWORD=$DB_PASSWORD
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesA && echo "Error: delete-map did not remove AllDataTypesA dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesB && echo "Error: delete-map did not remove AllDataTypesB dataset"
