#!/bin/bash
set -e

# This test:
#   - writes two datasets within the same AOI, one to an OSM API database and one to a Hoot API database 
#   - reads the OSM API dataset into the Hoot API database 
#   - conflates the two datasets together in hoot
#   - writes out a sql changeset file that contains the difference between the original OSM API database dataset and the conflated output  
#   - executes the changeset file SQL against the OSM API database
#   - reads out the final contents of the OSM API database and verifies them

REF_DATASET=test-files/DcGisRoads.osm
SEC_DATASET=test-files/DcTigerRoads.osm
AOI=-77.040,38.892,-77.033,38.896

#REF_DATASET=test-files/ToyTestA.osm
#SEC_DATASET=test-files/ToyTestB.osm
#AOI=-104.904,35.852,-104.895,38.856

echo "reference dataset: " $REF_DATASET
echo "secondary dataset: " $SEC_DATASET
echo "AOI: " $AOI

source conf/DatabaseConfig.sh

rm -rf test-output/cmd/glacial/ServiceOsmApiDbConflateTest
mkdir -p test-output/cmd/glacial/ServiceOsmApiDbConflateTest

echo ""
echo "STEP 1: Cleaning out the osm api db and initializing it for use..."
source scripts/SetupOsmApiDB.sh
export DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D hootapi.db.writer.email=HootApiDbWriterTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D hootapi.db.reader.email=HootApiDbWriterTest@hoottestcpp.org -D writer.include.debug=true --warn"

echo ""
echo "STEP 2: Writing the reference dataset to the osm api db..."
cp $REF_DATASET test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-ref-raw.osm
hoot convert $HOOT_OPTS test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-ref-raw.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-ref-ToBeAppliedToOsmApiDb.sql
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-ref-ToBeAppliedToOsmApiDb.sql

echo ""
echo "STEP 3: Reading the reference dataset out of the osm api db and writing it into a file (debugging purposes only)..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/3-ref-PulledFromOsmApiDb.osm

echo ""
echo "STEP 4: Querying out a cropped aoi for the reference dataset from the osm api db and writing it into a file (debugging purposes only)..."
hoot convert $HOOT_OPTS -D convert.bounding.box=$AOI $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/4-ref-cropped-PulledFromHootApiDb.osm

echo ""
echo "STEP 5: Writing a secondary dataset over the aoi to the hoot api db..."
cp $SEC_DATASET test-output/cmd/glacial/ServiceOsmApiDbConflateTest/5-sec-raw.osm
hoot convert $HOOT_OPTS -D convert.ops=hoot::MapCropper -D crop.bounds=$AOI test-output/cmd/glacial/ServiceOsmApiDbConflateTest/5-sec-raw.osm "$HOOT_DB_URL/5-sec-ServiceOsmApiDbConflateTest"

echo ""
echo "STEP 6: Reading the secondary dataset out of the hoot api db writing it into a file (debugging purposes only)..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/5-sec-ServiceOsmApiDbConflateTest" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/6-sec-cropped-PulledFromHootApiDb.osm

echo ""
echo "STEP 7: Conflating the two datasets..."
hoot conflate $HOOT_OPTS -D convert.bounding.box=$AOI -D conflate.use.data.source.ids.input.1=true -D conflate.use.data.source.ids.input.2=false $DB_URL "$HOOT_DB_URL/5-sec-ServiceOsmApiDbConflateTest" "$HOOT_DB_URL/7-conflated-ServiceOsmApiDbConflateTest"

echo ""
echo "STEP 8: Reading the conflated dataset out of the hoot api db and writing it into a file (debugging purposes only)..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/7-conflated-ServiceOsmApiDbConflateTest" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/8-conflated-PulledFromHootApiDb.osm

echo ""
echo "STEP 9: Writing a SQL changeset file that is the difference between the cropped reference input dataset and the conflated output..."
hoot derive-changeset $HOOT_OPTS -D changeset.user.id=1 -D convert.bounding.box=$AOI $DB_URL "$HOOT_DB_URL/7-conflated-ServiceOsmApiDbConflateTest" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/9-changeset-ToBeAppliedToOsmApiDb.osc.sql $DB_URL

echo ""
echo "STEP 10: Executing the changeset SQL on the osm api db..."
hoot apply-changeset test-output/cmd/glacial/ServiceOsmApiDbConflateTest/9-changeset-ToBeAppliedToOsmApiDb.osc.sql $DB_URL

#echo ""
#echo "STEP 11: Reading the contents of the osm api db for the specified aoi, writing it into a file, and verifying it (debugging purposes only)..."
#hoot convert -D convert.bounding.box=$AOI $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/11-cropped-output-PulledFromOsmApiDb.osm
#hoot is-match test-files/cmd/glacial/ServiceOsmApiDbConflateTest/cropped-output.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/6_a-cropped-output-PulledFromOsmApiDb.osm

echo ""
echo "STEP 12: Reading the entire contents of the osm api db, writing it into a file, and verifying it..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/12-output-PulledFromOsmApiDb.osm
hoot is-match test-files/cmd/glacial/ServiceOsmApiDbConflateTest/output.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/12-output-PulledFromOsmApiDb.osm

