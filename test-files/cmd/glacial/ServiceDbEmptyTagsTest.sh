#!/bin/bash
set -e

# This script recreates the problem in https://github.com/ngageoint/hootenanny/issues/168
source $HOOT_HOME/conf/DatabaseConfig.sh

export PGPASSWORD=$DB_PASSWORD

export HOOT_OPTS="-D services.db.writer.create.user=true -D services.db.writer.email=ServicesDbWriterTest@hoottestcpp.org -D services.db.writer.overwrite.map=true -D services.db.reader.email=ServicesDbWriterTest@hoottestcpp.org -D writer.include.debug=true --warn"

export DB_URL="postgresql://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

rm -rf test-output/cmd/ServiceDbEmptyTagTest
mkdir -p test-output/cmd/ServiceDbEmptyTagTest
hoot --conflate $HOOT_OPTS test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $DB_URL/ServicesDbEmptyTagTest

export MAP_ID=`psql -A -t -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER -c "SELECT id FROM maps WHERE display_name='ServicesDbEmptyTagTest'"`
export TB_COUNT=`psql -A -t -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER -c "SELECT COUNT(*) FROM current_ways_$MAP_ID WHERE tags?'bridge' OR tags?'tunnel'"`

echo "Tunnel or bridge count (should be zero): " $TB_COUNT
