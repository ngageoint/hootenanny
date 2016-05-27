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
#scripts/compareOsmXmlToOsmApiDbDumpWriter test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml test-files/ToyTestA.sql

######################################################
# TEST 2: BOUNDING BOX SELECTION FROM OSM API DB
######################################################

source scripts/CleanOsmApiDB.sh

createdb $AUTH $DB_NAME_OSMAPI

psql --quiet $AUTH -d $DB_NAME_OSMAPI -f test-files/bbox_test.sql

hoot convert -D convert.bounding.box=-106.5100000,38.3000000,-106.4000000,38.5000000 $DB_URL test-output/cmd/ServiceOsmApiDbTest/bboxOutput.osm

# perform a crude but effective comparison
cat test-output/cmd/ServiceOsmApiDbTest/bboxOutput.osm | grep 'tag k'
cat test-output/cmd/ServiceOsmApiDbTest/bboxOutput.osm | grep 'noderole'
cat test-output/cmd/ServiceOsmApiDbTest/bboxOutput.osm | grep 'wayrole'
cat test-output/cmd/ServiceOsmApiDbTest/bboxOutput.osm | grep 'relation'


