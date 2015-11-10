#!/bin/bash
set -e

#echo $PGDATABASE $PGHOST $PGPORT $PGUSER $PGPASSWORD

# clean out the database
source scripts/SetupOsmApiDB.sh

# setup DB variables for automation
source conf/DatabaseConfig.sh

export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD

# setup dirs
rm -rf test-output/cmd/ServiceOsmApiDbTest
mkdir -p test-output/cmd/ServiceOsmApiDbTest

#echo $PGDATABASE $PGHOST $PGPORT $PGUSER $PGPASSWORD

# Load the database with known data
psql --quiet $AUTH -d $DB_NAME_OSMAPI -f hoot-core-test/src/test/resources/ToyTestA.sql

export DB_URL="postgresql://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"

# do the read operation
echo "Performing read operation"
hoot convert -D services.db.writer.email=OsmApiInsert@hoot.local -D services.db.writer.create.user=true -D reader.add.source.datetime=false "$DB_URL" test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm

# create new maps file
scripts/generateIdMapXmlFromOsm test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml

# compare input to dumped
echo "Doing comparison"
scripts/compareOsmXmlToOsmApiDbDumpWriter test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml hoot-core-test/src/test/resources/ToyTestA.sql

