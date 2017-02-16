#!/bin/bash
set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh

export DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export PG_URL="dbname='$WFS_DB_NAME' host='$DB_HOST' port='$DB_PORT' user='$DB_USER' password='$DB_PASSWORD'"
export PGPASSWORD=$DB_PASSWORD
export outputname=service-export-test
export outputfolder=$HOOT_HOME/tmp/$outputname
export inputfile=DcGisRoads
export input=$inputfile_$outputname
export inputtype=db
export HOOT_OPTS="-D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D api.db.email=test@test.com -D writer.include.debug=true --warn"

# Ingest the data
hoot --convert $HOOT_OPTS test-files/$inputfile.osm $DB_URL/$input

# Delete any previous output
rm -rf $outputfolder

# Delete any previous WFS data store tables
SQL=$( psql -h $DB_HOST -t -A -d $WFS_DB_NAME -U $DB_USER -p $DB_PORT -c "SELECT 'DROP TABLE \"' || tablename || '\";' FROM pg_tables WHERE tablename like 'service_export_test\_%';" )
echo $SQL | psql -h localhost -d $WFS_DB_NAME -U $DB_USER -p $DB_PORT > /dev/null

# Test osm with no translation
export translation=""
export outputtype=osm
MAKEFLAGS= make -f $HOOT_HOME/scripts/services/osm2ogrscript > /dev/null

# Test shp with MPCP translation
export translation="translations/MGCP_TRD4.js"
export outputtype=shp
MAKEFLAGS= make -f $HOOT_HOME/scripts/services/osm2ogrscript > /dev/null

# Test gdb with TDSv6.1 translation
export translation="translations/TDSv61.js"
export outputtype=gdb
MAKEFLAGS= make -f $HOOT_HOME/scripts/services/osm2ogrscript > /dev/null

# Test wfs with TDSv4.0 translation
export translation="translations/TDSv40.js"
export outputtype=wfs
MAKEFLAGS= make -f $HOOT_HOME/scripts/services/osm2ogrscript > /dev/null


# Remove ingested data
hoot delete-map $HOOT_OPTS $DB_URL/$input

# Remove WFS data store tables
SQL=$( psql -h $DB_HOST -t -A -d $WFS_DB_NAME -U $DB_USER -p $DB_PORT -c "SELECT 'DROP TABLE \"' || tablename || '\";' FROM pg_tables WHERE tablename like 'service_export_test\_%';" )
echo $SQL | psql -h $DB_HOST -d $WFS_DB_NAME -U $DB_USER -p $DB_PORT > /dev/null

# Remove file output
rm -rf $outputfolder
