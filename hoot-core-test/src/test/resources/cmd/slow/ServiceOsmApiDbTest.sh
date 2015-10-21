#!/bin/bash
set -e

# clean out the database
scripts/SetupOsmApiDB.sh

# setup DB variables for automation
source conf/DatabaseConfig.sh

# setup dirs
rm -rf test-output/cmd/ServiceOsmApiDbTest
mkdir -p test-output/cmd/ServiceOsmApiDbTest

# Load the database with known data

# do the read operation
echo "Performing read operation"
hoot convert -D services.db.writer.email=OsmApiInsert@hoot.local -D services.db.writer.create.user=true -D reader.add.source.datetime=false "$DB_URL" test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm

# create new maps file
scripts/generateIdMapXmlFromOsm test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml

# compare input to dumped
#echo "Doing comparison"
scripts/compareOsmXmlToOsmApiDbDumpWriter test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml test-output/cmd/ServiceOsmApiDbTest/ToyTestA-dump.sql
