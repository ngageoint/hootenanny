#!/bin/bash

set -e

function pass {
    touch /dev/null
}

# SQL file and OSM API database outputs test

LOCAL_TEST_OUTPUT=test-output/cmd/glacial/BigConvertCmd
REF_FILES=test-files/cmd/glacial/BigConvertCmd
source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
scripts/database/CleanAndInitializeOsmApiDb.sh

echo "Removing previous outputs..."
hadoop fs -rmr DcGisRoads.osm.pbf &> /dev/null || pass
rm -rf $LOCAL_TEST_OUTPUT
mkdir -p $LOCAL_TEST_OUTPUT

# getting some strange results against the boston data...needs to be looked at again at some point
#echo "Converting input data to pbf..."
#hoot convert test-files/BostonSubsetRoadBuilding_FromOsm.osm $LOCAL_TEST_OUTPUT/BostonSubsetRoadBuilding.osm.pbf

echo "Copying input data to HDFS..."
hadoop fs -mkdir DcGisRoads.osm.pbf
hadoop fs -put test-files/DcGisRoads.pbf DcGisRoads.osm.pbf/

echo "Writing data to database..."
hoot big-convert --error -D changeset.user.id=1 -D osmapidb.bulk.inserter.output.files.copy.location=$LOCAL_TEST_OUTPUT/output.sql -D osmapidb.bulk.inserter.file.output.element.buffer.size=1000 -D big.convert.exec.sql.with.mapreduce=true DcGisRoads.osm.pbf $OSM_API_DB_URL

echo "Verifying output..."
# read the data back out of the db and compare
hoot convert --warn $OSM_API_DB_URL $LOCAL_TEST_OUTPUT/output.osm
hoot is-match --warn $LOCAL_TEST_OUTPUT/output.osm $REF_FILES/output.osm || cat $REF_FILES/output.osm

