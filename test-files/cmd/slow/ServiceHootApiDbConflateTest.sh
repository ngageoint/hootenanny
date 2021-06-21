#!/bin/bash
set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh
export PSQL_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export HOOT_EMAIL="ServiceHootApiDbConflateTest@hoottestcpp.org"

export HOOT_OPTS="-C Testing.conf -D hootapi.db.writer.create.user=true -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.overwrite.map=true -D writer.include.debug.tags=true --warn"
export CONFLATE_OPTS="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf"

export DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

rm -rf test-output/cmd/slow/ServiceHootApiDbConflateTest
mkdir -p test-output/cmd/slow/ServiceHootApiDbConflateTest

echo "Running select all conflate..."

hoot db-delete $HOOT_OPTS "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" &
wait
###
# I've run into strange situations where it seems like the whole files isn't being converted.
# It almost looks like the transaction isn't fully committed when the conflate op runs. I can't
# reproduce it reliably, but I would like to fix it if I can get more info. -JRS
# UPDATE: I think this is fixed by redmine #3588, but can't really prove a negative. -JRS
##
hoot convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesA.osm "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" &
hoot convert $HOOT_OPTS test-files/conflate/unified/AllDataTypesB.osm "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" &
wait

hoot conflate $HOOT_OPTS $CONFLATE_OPTS -D conflate.post.ops++="hoot::RemoveTagsVisitor;hoot::RemoveAttributesVisitor" -D tag.filter.keys="source:datetime" -D remove.attributes.visitor.types="changeset;timestamp" "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" test-output/cmd/slow/ServiceHootApiDbConflateTest/output1.osm
hoot diff test-files/cmd/slow/ServiceHootApiDbConflateTest/output1.osm test-output/cmd/slow/ServiceHootApiDbConflateTest/output1.osm

hoot db-delete $HOOT_OPTS "$DB_URL/AllDataTypesA-ServiceHootApiDbConflateTest" "$DB_URL/AllDataTypesB-ServiceHootApiDbConflateTest" &
wait

export PGPASSWORD=$DB_PASSWORD
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesA-ServiceHootApiDbConflateTest && echo "Error: db-delete did not remove AllDataTypesA-ServiceHootApiDbConflateTest dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw AllDataTypesB-ServiceHootApiDbConflateTest && echo "Error: db-delete did not remove AllDataTypesB-ServiceHootApiDbConflateTest dataset"

echo "Running select by bounds conflate..."

hoot db-delete $HOOT_OPTS "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" &
wait

hoot convert $HOOT_OPTS test-files/DcGisRoads.osm "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" &
hoot convert $HOOT_OPTS test-files/DcTigerRoads.osm "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" &
wait

hoot conflate $HOOT_OPTS $CONFLATE_OPTS -D match.creators=hoot::HighwayMatchCreator -D merger.creators=hoot::HighwayMergerCreator -D bounds=-77.04,38.8916,-77.03324,38.8958 -D conflate.post.ops++="hoot::RemoveTagsVisitor;hoot::RemoveAttributesVisitor" -D tag.filter.keys="source:datetime" -D remove.attributes.visitor.types="changeset;timestamp" "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" test-output/cmd/slow/ServiceHootApiDbConflateTest/output2.osm
hoot diff test-files/cmd/slow/ServiceHootApiDbConflateTest/output2.osm test-output/cmd/slow/ServiceHootApiDbConflateTest/output2.osm

hoot db-delete $HOOT_OPTS "$DB_URL/DcGisRoads-ServiceHootApiDbConflateTest" "$DB_URL/DcTigerRoads-ServiceHootApiDbConflateTest" &
wait

export PGPASSWORD=$DB_PASSWORD
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw DcGisRoads-ServiceHootApiDbConflateTest && echo "Error: db-delete did not remove DcGisRoads-ServiceHootApiDbConflateTest dataset"
psql -U $DB_USER -h $DB_HOST -p $DB_PORT -d $DB_NAME -c "select display_name from maps;" | grep -qw DcTigerRoads-ServiceHootApiDbConflateTest && echo "Error: db-delete did not remove DcTigerRoads-ServiceHootApiDbConflateTest dataset"

# Delete the user
PGPASSWORD=$DB_PASSWORD psql $PSQL_DB_AUTH -d $DB_NAME -c "DELETE FROM users WHERE email='$HOOT_EMAIL';" > /dev/null
