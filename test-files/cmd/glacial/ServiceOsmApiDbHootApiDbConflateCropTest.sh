#!/bin/bash
set -e

# Crop both OSM API DB and Hoot API DB data to the same AOI while streaming them in just before conflation, 
# then conflate conflate them, and then verify the output is cropped correctly.

source $HOOT_HOME/conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export HOOT_OPTS="--error -D hootapi.db.writer.create.user=true -D hootapi.db.writer.email=OsmApiDbConflate@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true -D hootapi.db.reader.email=OsmApiDbConflate@hoottestcpp.org -D reader.add.source.datetime=false -D postgresql.dumpfile.writer.add.circular.error.tag=true"

OUTPUT_DIR=test-output/cmd/glacial/ServiceOsmApiDbHootApiDbConflateCropTest
OSM_API_DB_INPUT=test-files/DcGisRoads.osm
OSM_API_DB_INPUT_EXPORT_TEMP=$OUTPUT_DIR/osm-api-db-input.osm
HOOT_API_DB_INPUT=test-files/DcTigerRoads.osm
HOOT_API_DB_INPUT_EXPORT_TEMP=$OUTPUT_DIR/hoot-api-db-input.osm
CONFLATED_EXPORT=$OUTPUT_DIR/conflated-export.osm
CROP_BOUNDS="-77.042,38.891,-77.032,38.898"  #slightly larger than the DC ellipse

mkdir -p $OUTPUT_DIR
rm -f $OSM_API_DB_INPUT_EXPORT $HOOT_API_DB_INPUT_EXPORT $CONFLATED_EXPORT

echo "Writing input 1 data to osm api db..."
source scripts/SetupOsmApiDB.sh force
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql
hoot convert $HOOT_OPTS $OSM_API_DB_INPUT tmp/DcGisRoads.sql
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f tmp/DcGisRoads.sql
echo "Exporting input 1 data from osm api db to temp file..."
hoot convert $HOOT_OPTS $OSM_API_DB_URL $OSM_API_DB_INPUT_EXPORT_TEMP

echo "Writing input 2 data to hoot api db..."
hoot convert $HOOT_OPTS $HOOT_API_DB_INPUT "$HOOT_DB_URL/DcTigerRoads"
echo "Exporting input 2 data from hoot api db to temp file..."
hoot convert $HOOT_OPTS "$HOOT_DB_URL/DcTigerRoads" $HOOT_API_DB_INPUT_EXPORT_TEMP

# Since we're mixing osm api db and hoot api db data here, we have to use a few extra hoot opts that aren't used in other hoot conflate
# situations.  Including stats here, since a bug related to running map cropping with it was discovered while writing this test.
echo "Cropping osm api/hoot api db data to same AOI and conflating it..."
hoot conflate $HOOT_OPTS -D conflate.use.data.source.ids=true -D osm.map.reader.factory.reader=hoot::OsmApiDbAwareHootApiDbReader -D osm.map.writer.factory.writer=hoot::OsmApiDbAwareHootApiDbWriter -D osmapidb.id.aware.url=$OSM_API_DB_URL -D convert.bounding.box=$CROP_BOUNDS -D conflate.load.post.ops="hoot::MapCropper" -D crop.bounds=$CROP_BOUNDS $OSM_API_DB_URL "$HOOT_DB_URL/DcTigerRoads" "$HOOT_DB_URL/DcConflated" --stats $OUTPUT_DIR/stats

# We're just verifying the extents with the command output and not running hoot is-match on the output files.  
# OSM API DB/Hoot API DB conflation output is verified in other tests.
echo "Reading conflated data back out of the hoot api db and checking extents..."
# The extent of both of the exported files for the inputs should be the same as the extent of the originally imported files - 
# default extent for both of them is -77.0551,38.8845,-77.0281,38.9031
echo "OSM API DB input 1 extent: " 
hoot map-extent $OSM_API_DB_INPUT
echo "OSM API DB input 1 exported extent: " 
hoot map-extent $OSM_API_DB_INPUT_EXPORT_TEMP
echo "Hoot API DB input 2 extent: " 
hoot map-extent $HOOT_API_DB_INPUT
echo "Hoot API DB input 2 exported extent: " 
hoot map-extent $HOOT_API_DB_INPUT_EXPORT_TEMP
hoot convert $HOOT_OPTS "$HOOT_DB_URL/DcConflated" $CONFLATED_EXPORT
# extent of output should be equal to that of the crop bounds
echo "Crop bounds: " $CROP_BOUNDS
echo "Conflated data output extent: " 
hoot map-extent $CONFLATED_EXPORT
