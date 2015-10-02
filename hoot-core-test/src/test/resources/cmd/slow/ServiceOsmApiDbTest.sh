#!/bin/bash
set -e

# clean out the database
scripts/SetupOsmApiDB.sh

# setup DB variables for automation
source conf/DatabaseConfig.sh

# setup dirs
rm -rf test-output/cmd/ServiceOsmApiDbTest
mkdir -p test-output/cmd/ServiceOsmApiDbTest

# setup args
export HOOT_OPTS="-D services.db.writer.create.user=true -D services.db.writer.email=OsmApiInsert@hoot.local -D reader.add.source.datetime=false -D services.db.writer.output.id.mappings=test-output/cmd/ServiceOsmApiDbTest/idmaps.xml "

export DB_URL="postgresql://$PGUSER:$PGPASSWORD@$PGHOST:$PGPORT/$PGDATABASE"

# do the write operation
echo "Performing write operation"
hoot convert $HOOT_OPTS test-files/ToyTestA.osm "$DB_URL"
#wait

# dump the database
#echo "Doing postgres dump operation"
pg_dump -U $PGUSER -a > test-output/cmd/ServiceOsmApiDbTest/ToyTestA-dump.sql
#wait

# compare input to dumped
#echo "Doing comparison"
scripts/compareOsmXmlToOsmApiDbDump test-files/ToyTestA.osm test-output/cmd/ServiceOsmApiDbTest/idmaps.xml test-output/cmd/ServiceOsmApiDbTest/ToyTestA-dump.sql

# do the read operation
echo "Performing read operation"
hoot convert -D services.db.writer.email=OsmApiInsert@hoot.local -D services.db.writer.create.user=true -D reader.add.source.datetime=false "$DB_URL" test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm

# create new maps file
scripts/generateIdMapXmlFromOsm test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml

# compare input to dumped
#echo "Doing comparison"
scripts/compareOsmXmlToOsmApiDbDumpWriter test-output/cmd/ServiceOsmApiDbTest/ToyTestA-out.osm test-output/cmd/ServiceOsmApiDbTest/idmaps2.xml test-output/cmd/ServiceOsmApiDbTest/ToyTestA-dump.sql
