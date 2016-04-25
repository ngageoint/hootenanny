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
echo "reference dataset: " $REF_DATASET
SEC_DATASET=test-files/DcTigerRoads.osm
echo "secondary dataset: " $SEC_DATASET
AOI=-77.040,38.892,-77.033,38.896
echo "AOI: " $AOI

source conf/DatabaseConfig.sh

rm -rf test-output/cmd/glacial/ServiceOsmApiDbConflateTest
mkdir -p test-output/cmd/glacial/ServiceOsmApiDbConflateTest

ctr=1
echo ""
echo "STEP "$ctr": Cleaning out the osm api db and initializing it for use..."
source scripts/SetupOsmApiDB.sh
export DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D hootapi.db.writer.email=HootApiDbWriterTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D hootapi.db.reader.email=HootApiDbWriterTest@hoottestcpp.org -D writer.include.debug=true --warn"

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Writing the reference dataset to an osm api db..."
cp $REF_DATASET test-output/cmd/glacial/ServiceOsmApiDbConflateTest/1-ref-raw.osm
hoot convert $HOOT_OPTS test-output/cmd/glacial/ServiceOsmApiDbConflateTest/1-ref-raw.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-ref-ToBeAppliedToOsmApiDb.sql
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-output/cmd/glacial/ServiceOsmApiDbConflateTest/2-ref-ToBeAppliedToOsmApiDb.sql

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Reading the reference dataset back out of the osm api db into a file..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/3-ref-PulledFromOsmApiDb.osm

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Querying out a cropped aoi for the reference dataset from the osm api db and writing it into a hoot api db..."
hoot convert $HOOT_OPTS -D convert.bounding.box=$AOI $DB_URL "$HOOT_DB_URL/ServiceOsmApiDbConflateTestRef"

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Reading the cropped reference dataset back out of the hoot api db into a file..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestRef" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/4-ref-PulledFromHootApiDb-cropped.osm

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Writing a secondary dataset over the aoi to the hoot api db..."
cp $SEC_DATASET test-output/cmd/glacial/ServiceOsmApiDbConflateTest/5-sec-raw.osm
hoot convert $HOOT_OPTS -D convert.ops=hoot::MapCropper -D crop.bounds=$AOI test-output/cmd/glacial/ServiceOsmApiDbConflateTest/5-sec-raw.osm "$HOOT_DB_URL/ServiceOsmApiDbConflateTestSec"

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Reading the second dataset back out of the hoot api db into a file..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestSec" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/6-sec-PulledFromHootApiDb-cropped.osm

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Conflating the two datasets..."
hoot conflate $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestRef" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestSec" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated"

ctr=$((ctr+1))
echo ""
echo "Reading the conflated contents back out of the hoot api db..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/7-PulledFromHootApiDb-conflated.osm

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Writing a SQL changeset file that is the difference between the cropped reference input dataset and the conflated output..."
hoot derive-changeset $HOOT_OPTS -D changeset.user.id=1 "$HOOT_DB_URL/ServiceOsmApiDbConflateTestRef" "$HOOT_DB_URL/ServiceOsmApiDbConflateTestConflated" test-output/cmd/glacial/ServiceOsmApiDbConflateTest/8-changeset-ToBeAppliedToOsmApiDb.osc.sql $DB_URL

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Writing the changeset SQL back to the osm api db..."
hoot apply-changeset test-output/cmd/glacial/ServiceOsmApiDbConflateTest/8-changeset-ToBeAppliedToOsmApiDb.osc.sql $DB_URL

#ctr=$((ctr+1))
#echo ""
#echo "Reading the contents of the osm api db for the specified aoi into a file and verifying it..."
#hoot convert -D convert.bounding.box=$AOI $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/9-final-output-PulledFromOsmApiDb-cropped.osm
#hoot is-match test-files/cmd/glacial/ServiceOsmApiDbConflateTest/cropped-output.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/9-final-output-PulledFromOsmApiDb-cropped.osm

ctr=$((ctr+1))
echo ""
echo "STEP "$ctr": Reading the entire contents of the osm api db into a file and verifying it..."
hoot convert $DB_URL test-output/cmd/glacial/ServiceOsmApiDbConflateTest/10-final-output-PulledFromOsmApiDb.osm
hoot is-match test-files/cmd/glacial/ServiceOsmApiDbConflateTest/output.osm test-output/cmd/glacial/ServiceOsmApiDbConflateTest/10-final-output-PulledFromOsmApiDb.osm

