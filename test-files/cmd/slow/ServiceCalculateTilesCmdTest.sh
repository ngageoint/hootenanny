#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/ServiceCalculateTilesCmdTest
GOLD_FILES_DIR=test-files/cmd/slow/CalculateTilesCmdTest
mkdir -p $OUTPUT_DIR 

# convert.bounding.box only works with db inputs

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert --warn -D api.db.email=test@test.com -D changeset.user.id=1 -D osmapidb.bulk.inserter.validate.data=true test-files/DcGisRoads.osm $OSM_API_DB_URL
hoot convert --warn -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true -D api.db.email=test@test.com test-files/DcTigerRoads.osm $HOOT_DB_URL"/DcTigerRoads"
hoot calculate-tiles --warn -D api.db.email=test@test.com -D convert.bounding.box="-77.04856,38.8855,-77.0292,38.899" $OSM_API_DB_URL";"$HOOT_DB_URL"/DcTigerRoads" $OUTPUT_DIR/output-cropped.geojson 1000 0.001
diff $GOLD_FILES_DIR/output-cropped.geojson $OUTPUT_DIR/output-cropped.geojson

