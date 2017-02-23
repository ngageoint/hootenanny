#!/bin/bash
set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh

export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D api.db.email=ServiceHootApiDbConflateTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D writer.include.debug=true --warn"

export DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

rm -rf test-output/cmd/ServiceHootApiDbConflateTest
mkdir -p test-output/cmd/ServiceHootApiDbConflateTest

echo "Running select all conflate..."

hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" &
hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" &
wait
###
# I've run into strange situations where it seems like the whole files isn't being converted.
# It almost looks like the transaction isn't fully committed when the conflat op runs. I can't
# reproduce it reliably, but I would like to fix it if I can get more info. -JRS
# I think this is fixed by #3588, but can't really prove a negative. -JRS
##
hoot convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesA.osm "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" &
hoot convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesB.osm "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" &
wait

hoot conflate $HOOT_OPTS -D unify.post.ops="hoot::SuperfluousNodeRemover;hoot::SmallWayMerger;hoot::RemoveTagVisitor;hoot::RemoveAttributeVisitor" -D remove.tag.visitor.keys="source:datetime" -D remove.attribute.visitor.types="changeset;timestamp" "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" test-output/cmd/ServiceHootApiDbConflateTest/output1.osm
hoot is-match test-files/cmd/slow/ServiceHootApiDbConflateTest/output1.osm test-output/cmd/ServiceHootApiDbConflateTest/output1.osm

hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" &
hoot delete-map $HOOT_OPTS "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" &
wait

export PGPASSWORD=$DB_PASSWORD
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesA-ServiceHootApiDbConflateTest && echo "Error: delete-map did not remove AllDataTypesA-ServiceHootApiDbConflateTest dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesB-ServiceHootApiDbConflateTest && echo "Error: delete-map did not remove AllDataTypesB-ServiceHootApiDbConflateTest dataset"

echo "Running select by bounds conflate..."

hoot delete-map $HOOT_OPTS "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" &
hoot delete-map $HOOT_OPTS "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" &
wait

hoot convert $HOOT_OPTS test-files/DcGisRoads.osm "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" &
hoot convert $HOOT_OPTS test-files/DcTigerRoads.osm "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" &
wait

hoot conflate $HOOT_OPTS -D convert.bounding.box=-77.04,38.8916,-77.03324,38.8958 -D unify.post.ops="hoot::SuperfluousNodeRemover;hoot::SmallWayMerger;hoot::RemoveTagVisitor;hoot::RemoveAttributeVisitor" -D remove.tag.visitor.keys="source:datetime" -D remove.attribute.visitor.types="changeset;timestamp" "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" test-output/cmd/ServiceHootApiDbConflateTest/output2.osm
hoot is-match test-files/cmd/slow/ServiceHootApiDbConflateTest/output2.osm test-output/cmd/ServiceHootApiDbConflateTest/output2.osm

hoot delete-map $HOOT_OPTS "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" &
hoot delete-map $HOOT_OPTS "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" &
wait

export PGPASSWORD=$DB_PASSWORD
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw DcGisRoads-ServiceHootApiDbConflateTest && echo "Error: delete-map did not remove DcGisRoads-ServiceHootApiDbConflateTest dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw DcTigerRoads-ServiceHootApiDbConflateTest && echo "Error: delete-map did not remove DcTigerRoads-ServiceHootApiDbConflateTest dataset"



