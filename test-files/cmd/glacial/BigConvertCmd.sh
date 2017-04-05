#!/bin/bash

set -e

function pass {
    touch /dev/null
}

# SQL file and OSM API database outputs test

#HDFS_TEST_OUTPUT=test-output/BigConvertCmd
LOCAL_TEST_OUTPUT=test-output/cmd/glacial/BigConvertCmd
REF_FILES=test-files/cmd/glacial/BigConvertCmd
source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
scripts/database/CleanAndInitializeOsmApiDb.sh

echo "Removing previous outputs..."
#hadoop fs -rmr BostonSubsetRoadBuilding.osm.pbf &> /dev/null || pass
hadoop fs -rmr DcGisRoads.osm.pbf &> /dev/null || pass
rm -rf $LOCAL_TEST_OUTPUT
mkdir -p $LOCAL_TEST_OUTPUT
#hadoop fs -rmr $HDFS_TEST_OUTPUT &> /dev/null || pass
#hadoop fs -mkdir $HDFS_TEST_OUTPUT

#echo "Converting input data to pbf..."
#hoot convert test-files/BostonSubsetRoadBuilding_FromOsm.osm $LOCAL_TEST_OUTPUT/BostonSubsetRoadBuilding.osm.pbf

echo "Copying input data to HDFS..."
#hadoop fs -mkdir BostonSubsetRoadBuilding.osm.pbf
#hadoop fs -put $LOCAL_TEST_OUTPUT/BostonSubsetRoadBuilding.osm.pbf BostonSubsetRoadBuilding.osm.pbf/
hadoop fs -mkdir DcGisRoads.osm.pbf
hadoop fs -put test-files/DcGisRoads.pbf DcGisRoads.osm.pbf/

echo "Writing data to database..."
#hoot big-convert --info -D changeset.user.id=1 -D osmapidb.bulk.writer.output.files.copy.location=$LOCAL_TEST_OUTPUT/output.sql -D osmapidb.bulk.writer.file.output.element.buffer.size=100 BostonSubsetRoadBuilding.osm.pbf $OSM_API_DB_URL
hoot big-convert --error -D changeset.user.id=1 -D osmapidb.bulk.writer.output.files.copy.location=$LOCAL_TEST_OUTPUT/output.sql -D osmapidb.bulk.writer.file.output.element.buffer.size=100 DcGisRoads.osm.pbf $OSM_API_DB_URL

echo "Verifying output..."
# read the data back out of the db and compare
hoot convert --warn $OSM_API_DB_URL $LOCAL_TEST_OUTPUT/output.osm
hoot is-match --warn $LOCAL_TEST_OUTPUT/output.osm $REF_FILES/output.osm || cat $REF_FILES/output.osm

