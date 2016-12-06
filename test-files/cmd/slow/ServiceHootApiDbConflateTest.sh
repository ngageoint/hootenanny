#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh

export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D hootapi.db.writer.email=ServiceHootApiDbConflateTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D hootapi.db.reader.email=ServiceHootApiDbConflateTest@hoottestcpp.org -D writer.include.debug=true --warn"

export DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

rm -rf test-output/cmd/ServiceHootApiDbConflateTest
mkdir -p test-output/cmd/ServiceHootApiDbConflateTest

# select all

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

hoot conflate $HOOT_OPTS "$DB_URL/AllDataTypesA" "$DB_URL/AllDataTypesB" test-output/cmd/ServiceHootApiDbConflateTest/output1.osm
# Do a very coarse comparison check.
hoot stats --quick test-output/cmd/ServiceHootApiDbConflateTest/output1.osm

hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesA" &
hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesB" &
wait

export PGPASSWORD=$DB_PASSWORD
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesA && echo "Error: delete-map did not remove AllDataTypesA dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesB && echo "Error: delete-map did not remove AllDataTypesB dataset"

# select by bounds

hoot delete-map $HOOT_OPTS "$DB_URL/DcGisRoads" &
hoot delete-map $HOOT_OPTS "$DB_URL/DcTigerRoads" &
wait

hoot convert $HOOT_OPTS test-files/DcGisRoads.osm "$DB_URL/DcGisRoads" &
hoot convert $HOOT_OPTS test-files/DcTigerRoads.osm "$DB_URL/DcTigerRoads" &
wait

hoot conflate $HOOT_OPTS -D convert.bounding.box=-77.04,38.8916,-77.03324,38.8958 -D unify.post.ops="hoot::SuperfluousNodeRemover;hoot::SmallWayMerger;hoot::RemoveTagVisitor;hoot::RemoveAttributeVisitor" -D remove.tag.visitor.keys="source:datetime" -D remove.attribute.visitor.types="changeset;timestamp" "$DB_URL/DcGisRoads" "$DB_URL/DcTigerRoads" test-output/cmd/ServiceHootApiDbConflateTest/output2.osm
hoot is-match test-files/cmd/slow/ServiceHootApiDbConflateTest/output2.osm test-output/cmd/ServiceHootApiDbConflateTest/output2.osm

hoot delete-map $HOOT_OPTS "$DB_URL/DcGisRoads" &
hoot delete-map $HOOT_OPTS "$DB_URL/DcTigerRoads" &
wait

export PGPASSWORD=$DB_PASSWORD
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw DcGisRoads && echo "Error: delete-map did not remove DcGisRoads dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw DcTigerRoads && echo "Error: delete-map did not remove DcTigerRoads dataset"



