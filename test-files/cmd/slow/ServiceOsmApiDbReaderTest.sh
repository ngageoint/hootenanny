#!/bin/bash
set -e

#echo $PGDATABASE $PGHOST $PGPORT $PGUSER $PGPASSWORD

# clean out the database
source scripts/SetupOsmApiDB.sh force

# setup DB variables for automation
source conf/DatabaseConfig.sh

export AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

# setup dirs
OUTPUT_DIR=test-output/cmd/slow/ServiceOsmApiDbReaderTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

######################################################
# TEST 1: SELECTION FROM OSM API DB
######################################################

#echo $PGDATABASE $PGHOST $PGPORT $PGUSER $PGPASSWORD
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/ToyTestA.sql

export DB_URL="osmapidb://$DB_USER_OSMAPI:$DB_PASSWORD_OSMAPI@$DB_HOST_OSMAPI:$DB_PORT_OSMAPI/$DB_NAME_OSMAPI"

hoot convert "$DB_URL" $OUTPUT_DIR/output1.osm
hoot is-match test-files/cmd/slow/ServiceOsmApiDbReaderTest/output1.osm $OUTPUT_DIR/output1.osm

######################################################
# TEST 2: BOUNDING BOX SELECTION FROM OSM API DB
######################################################

source conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPIt

source scripts/SetupOsmApiDB.sh force
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

hoot convert --error test-files/DcGisRoads.osm $OUTPUT_DIR/DcGisRoads.sql
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f $OUTPUT_DIR/DcGisRoads.sql

hoot convert --error $HOOT_OPTS -D convert.bounding.box=-77.04,38.8916,-77.03324,38.8958 $OSM_API_DB_URL $OUTPUT_DIR/output2.osm
hoot is-match test-files/cmd/slow/ServiceOsmApiDbReaderTest/output2.osm $OUTPUT_DIR/output2.osm


