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
rm -rf test-output/cmd/ServiceOsmApiDbTest
mkdir -p test-output/cmd/ServiceOsmApiDbTest


######################################################
# TEST 1: SELECTION FROM OSM API DB
######################################################

# Load the database with known data
#echo $PGDATABASE $PGHOST $PGPORT $PGUSER $PGPASSWORD
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/ToyTestA.sql

export DB_URL="osmapidb://$DB_USER_OSMAPI:$DB_PASSWORD_OSMAPI@$DB_HOST_OSMAPI:$DB_PORT_OSMAPI/$DB_NAME_OSMAPI"

# do the read operation
echo "Performing read operation"
hoot convert "$DB_URL" test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm

# compare input to dumped
echo "Doing comparison"
# TODO: why is this commented out?
#scripts/compareOsmXmlToOsmApiDbDumpWriter test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml test-files/ToyTestA.sql

######################################################
# TEST 2: BOUNDING BOX SELECTION FROM OSM API DB
######################################################

source conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
OUTPUT_DIR=test-output/cmd/slow/ServiceOsmApiDbReaderTest
mkdir -p $OUTPUT_DIR

source scripts/SetupOsmApiDB.sh force
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

hoot convert --error test-files/DcGisRoads.osm $OUTPUT_DIR/DcGisRoads.sql
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f $OUTPUT_DIR/DcGisRoads.sql

hoot convert --error $HOOT_OPTS -D convert.bounding.box=-77.04,38.8916,-77.03324,38.8958 $OSM_API_DB_URL $OUTPUT_DIR/output.osm
hoot is-match test-files/cmd/slow/ServiceOsmApiDbReaderTest/output.osm $OUTPUT_DIR/output.osm


