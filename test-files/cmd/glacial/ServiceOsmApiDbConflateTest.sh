#!/bin/bash
set -e

# This test:
#   - writes two datasets with the same AOI, one to an OSM API database and one to a Hoot API database 
#   - reads the OSM API dataset into the Hoot API database 
#   - conflates the two datasets together in hoot
#   - writes out a sql changeset file that contains the difference between the original OSM API database dataset and the conflated output  
#   - executes the changeset file writes on the OSM API database
#   - reads out the final contents of the OSM API database and verified thems

source conf/DatabaseConfig.sh

rm -rf test-output/cmd/glacial/ServiceOsmApiDbConflateTest
mkdir -p test-output/cmd/glacial/ServiceOsmApiDbConflateTest

echo ""
echo "Cleaning out the osm api db and setting it up with an initial single user..."
source scripts/SetupOsmApiDB.sh force
export DB_URL="osmapidb://$DB_USER_OSMAPI:$DB_PASSWORD_OSMAPI@$DB_HOST_OSMAPI:$DB_PORT_OSMAPI/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D hootapi.db.writer.email=HootApiDbWriterTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D hootapi.db.reader.email=HootApiDbWriterTest@hoottestcpp.org -D writer.include.debug=true --warn"

AOI=-77.040,38.892,-77.033,38.896

echo ""
echo "Writing a dataset to an osm api db..."
cp test-files/DcGisRoads.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/1-DcGisRoads-raw.osm
hoot convert $HOOT_OPTS test-output/cmd/glacial/ServiceOsmApiDbConflateTest/1-DcGisRoads-raw.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-DcGisRoads-ToBeAppliedToOsmApiDb.sql
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-DcGisRoads-ToBeAppliedToOsmApiDb.sql

echo ""
echo "Reading the contents back out of the osm api db..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/3-DcGisRoads-PulledFromOsmApiDb.osm

echo ""
echo "Querying a subset aoi for the data out of the osm api db into a hoot api db..."
hoot convert $HOOT_OPTS -D convert.bounding.box=$AOI $DB_URL "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads"

echo ""
echo "Reading the first dataset contents back out of the hoot api db..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/4-DcGisRoads-PulledFromHootApiDb-cropped.osm

echo ""
echo "Writing a second dataset over the same aoi to a hoot api db..."
cp test-files/DcTigerRoads.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/5-DcTigerRoads-raw.osm
hoot convert $HOOT_OPTS -D convert.ops=hoot::MapCropper -D crop.bounds=$AOI test-output/cmd/glacial/ServiceOsmApiDbConflateTest/5-DcTigerRoads-raw.osm "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcTigerRoads"

echo ""
echo "Reading the second dataset contents back out of the hoot api db..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcTigerRoads" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/6-DcTigerRoads-PulledFromHootApiDb-cropped.osm

echo ""
echo "Conflating the two datasets..."
hoot conflate $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcTigerRoads" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated"

echo ""
echo "Reading the conflated contents back out of the hoot api db..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/7-PulledFromHootApiDb-conflated.osm

echo ""
echo "Creating a changeset that is the diff between the osm api db input data and the conflated output..."
hoot derive-changeset $HOOT_OPTS -D changeset.user.id=1 "$HOOT_DB_URL/ServiceOsmApiDbConflateTestDcGisRoads" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/8-changeset-ToBeAppliedToOsmApiDb.osc.sql $DB_URL

echo ""
echo "Writing the changeset back to the osm api db..."
hoot apply-changeset test-output/cmd/glacial/ServiceOsmApiDbConflateTest/8-changeset-ToBeAppliedToOsmApiDb.osc.sql $DB_URL

echo ""
echo "Reading the final contents back out of the osm api db and verifying the final output..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/9-final-output-PulledFromOsmApiDb.osm
hoot is-match test-files/cmd/glacial/ServiceOsmApiDbConflateTest/output.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/9-final-output-PulledFromOsmApiDb.osm

