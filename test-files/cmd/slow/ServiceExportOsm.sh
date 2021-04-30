#!/bin/bash
set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh

export DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export PSQL_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD
export outputname=service-export-test
export outputfolder=$HOOT_HOME/tmp/$outputname
export inputfile=DcGisRoads
export input=$inputfile_$outputname
export inputtype=db
export TEST_EMAIL="ServiceExportOsmTest@hootenanny.com"
export HOOT_OPTS="-C Testing.conf -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D api.db.email=${TEST_EMAIL} -D writer.include.debug.tags=true --warn"

echo "Ingest the data"
hoot convert $HOOT_OPTS test-files/$inputfile.osm $DB_URL/$input

# Delete any previous output
rm -rf $outputfolder

echo "Test osm with no translation"
export translation=""
export outputtype=osm
MAKEFLAGS= make -f $HOOT_HOME/scripts/core/osm2ogrscript > /dev/null

echo "Test shp with MGCP translation"
export translation="translations/MGCP_TRD4.js"
export outputtype=shp
MAKEFLAGS= make -f $HOOT_HOME/scripts/core/osm2ogrscript > /dev/null

echo "Test gdb with TDSv6.1 translation"
export translation="translations/TDSv61.js"
export outputtype=gdb
MAKEFLAGS= make -f $HOOT_HOME/scripts/core/osm2ogrscript > /dev/null

echo "Remove ingested data"
hoot db-delete $HOOT_OPTS $DB_URL/$input

# Remove file output
rm -rf $outputfolder

# Delete the user
PGPASSWORD=$DB_PASSWORD psql $PSQL_DB_AUTH -d $DB_NAME  -c "DELETE FROM users WHERE email='${TEST_EMAIL}';" > /dev/null
