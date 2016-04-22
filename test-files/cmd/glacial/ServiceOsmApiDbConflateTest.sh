#!/bin/bash
set -e

source conf/DatabaseConfig.sh

rm -rf test-output/cmd/glacial/ServiceOsmApiDbConflateTest
mkdir -p test-output/cmd/glacial/ServiceOsmApiDbConflateTest

echo ""
echo "Cleaning out the osm api db and setting it up with an initial single user..."
source scripts/SetupOsmApiDB.sh
export DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D hootapi.db.writer.email=HootApiDbWriterTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D hootapi.db.reader.email=HootApiDbWriterTest@hoottestcpp.org -D writer.include.debug=true --warn"

AOI=-77.040,38.892,-77.033,38.896

echo ""
echo "Writing a dataset to an osm api db..."
hoot convert $HOOT_OPTS test-files/DcGisRoads.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/DcGisRoads.sql
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-output/cmd/glacial/ServiceOsmApiDbConflateTest/DcGisRoads.sql

echo ""
echo "Reading the contents back out of the osm api db for debugging purposes..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/DcGisRoads.osm

echo ""
echo "Querying a subset aoi for the data out of the osm api db into a hoot api db..."
hoot convert $HOOT_OPTS -D convert.bounding.box=$AOI $DB_URL "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads"

echo ""
echo "Reading the first dataset contents back out of the hoot api db for debugging purposes..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/DcGisRoads-cropped.osm

echo ""
echo "Writing a second dataset over the same aoi to a hoot api db..."
hoot delete-map $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcTigerRoads"
hoot convert $HOOT_OPTS -D convert.ops=hoot::MapCropper -D crop.bounds=$AOI test-files/DcTigerRoads.osm "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcTigerRoads"

echo ""
echo "Reading the second dataset contents back out of the hoot api db for debugging purposes..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcTigerRoads" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/DcTigerRoads-cropped.osm

echo ""
echo "Conflating the two datasets..."
hoot conflate $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcTigerRoads" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated"

echo ""
echo "Reading the conflated contents back out of the hoot api db for debugging purposes..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/conflated.osm

echo ""
echo "Creating a changeset that is the diff between the osm api db input data and the conflated output..."
hoot derive-changeset $HOOT_OPTS -D changeset.user.id=1 "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/changeset.osc.sql $DB_URL

#TODO: dying here b/c the database claims we're writing nodes that already exist (node with same id and same version already in database)
echo ""
echo "Writing the changeset back to the osm api db..."
hoot apply-changeset test-output/cmd/glacial/ServiceOsmApiDbConflateTest/changeset.osc.sql $DB_URL

echo ""
echo "Reading the final contents back out of the osm api db and verifying..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/final-output.osm
hoot is-match test-files/cmd/glacial/ServiceOsmApiDbConflateTest/output.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/final-output.osm

