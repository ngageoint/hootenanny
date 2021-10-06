#!/bin/bash
set -e

REF_DIR=test-files/cmd/slow/ServiceMultiaryPoiIngestCmdTest
OUTPUT_DIR=test-output/cmd/slow/ServiceMultiaryPoiIngestCmdTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

source conf/database/DatabaseConfig.sh
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
PSQL_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_EMAIL="ServiceMultiaryPoiIngestCmdTest@hoottestcpp.org"
# set the max elements per map in such a way that the partial reading occurs
HOOT_OPTS="--warn -C Testing.conf -D uuid.helper.repeatable=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D max.elements.per.partial.map=2"

GOLD_OUTPUT=$REF_DIR/allCountries-geonames-output.osm
GOLD_ADD_CHANGESET=$REF_DIR/allCountries-geonames-changeset-add.spark.1
GOLD_DELETE_CHANGESET=$REF_DIR/allCountries-geonames-changeset-delete.spark.1

# GEONAMES

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.geonames
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.geonames
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-geonames-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-geonames-changeset.spark.1
CHANGESET_ADD_OUTPUT=$OUTPUT_DIR/allCountries-geonames-changeset-add.spark.1
CHANGESET_DELETE_OUTPUT=$OUTPUT_DIR/allCountries-geonames-changeset-delete.spark.1
TRANSLATION_SCRIPT=translations/GeoNames_to_OSM.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot db-delete $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING GEONAMES REFERENCE DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING GEONAMES NEW DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING GEONAMES REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING GEONAMES REFERENCE LAYER OUTPUT..."
echo ""
hoot diff $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING GEONAMES ADD CHANGESET OUTPUT..."
echo ""
diff $GOLD_ADD_CHANGESET $CHANGESET_ADD_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING GEONAMES DELETE CHANGESET OUTPUT..."
echo ""
diff $GOLD_DELETE_CHANGESET $CHANGESET_DELETE_OUTPUT

GOLD_OUTPUT=$REF_DIR/allCountries-osmpbf-output.osm
GOLD_ADD_CHANGESET=$REF_DIR/allCountries-osmpbf-changeset-add.spark.1
GOLD_DELETE_CHANGESET=$REF_DIR/allCountries-osmpbf-changeset-delete.spark.1

# OSM PBF

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.osm.pbf
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.osm.pbf
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-osmpbf-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-osmpbf-changeset.spark.1
CHANGESET_ADD_OUTPUT=$OUTPUT_DIR/allCountries-osmpbf-changeset-add.spark.1
CHANGESET_DELETE_OUTPUT=$OUTPUT_DIR/allCountries-osmpbf-changeset-delete.spark.1
TRANSLATION_SCRIPT=translations/OSM_Ingest.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot db-delete $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING OSM PBF REFERENCE DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING OSM PBF NEW DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING OSM PBF REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING OSM PBF REFERENCE LAYER OUTPUT..."
echo ""
hoot diff $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING OSM PBF ADD CHANGESET OUTPUT..."
echo ""
diff $GOLD_ADD_CHANGESET $CHANGESET_ADD_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING OSM PBF DELETE CHANGESET OUTPUT..."
echo ""
diff $GOLD_DELETE_CHANGESET $CHANGESET_DELETE_OUTPUT

GOLD_OUTPUT=$REF_DIR/allCountries-osmxml-output.osm
GOLD_ADD_CHANGESET=$REF_DIR/allCountries-osmxml-changeset-add.spark.1
GOLD_DELETE_CHANGESET=$REF_DIR/allCountries-osmxml-changeset-delete.spark.1

# OSM XML

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.osm
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.osm
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-osmxml-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-osmxml-changeset.spark.1
CHANGESET_ADD_OUTPUT=$OUTPUT_DIR/allCountries-osmxml-changeset-add.spark.1
CHANGESET_DELETE_OUTPUT=$OUTPUT_DIR/allCountries-osmxml-changeset-delete.spark.1
TRANSLATION_SCRIPT=translations/OSM_Ingest.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot db-delete $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING OSM XML REFERENCE DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING OSM XML NEW DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING OSM XML REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING OSM XML REFERENCE LAYER OUTPUT..."
echo ""
hoot diff $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING OSM XML ADD CHANGESET OUTPUT..."
echo ""
diff $GOLD_ADD_CHANGESET $CHANGESET_ADD_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING OSM XML DELETE CHANGESET OUTPUT..."
echo ""
diff $GOLD_DELETE_CHANGESET $CHANGESET_DELETE_OUTPUT

GOLD_OUTPUT=$REF_DIR/allCountries-shp-output.osm
GOLD_ADD_CHANGESET=$REF_DIR/allCountries-shp-changeset-add.spark.1
GOLD_DELETE_CHANGESET=$REF_DIR/allCountries-shp-changeset-delete.spark.1

# SHP

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.shp/Points.shp
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.shp/Points.shp
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-shp-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-shp-changeset.spark.1
CHANGESET_ADD_OUTPUT=$OUTPUT_DIR/allCountries-shp-changeset-add.spark.1
CHANGESET_DELETE_OUTPUT=$OUTPUT_DIR/allCountries-shp-changeset-delete.spark.1
TRANSLATION_SCRIPT=translations/OSM_Ingest.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot db-delete $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING SHP REFERENCE DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS -D ogr.reader.node.id.field.name=geonameid $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING SHP NEW DATASET..."
echo ""
hoot multiary-poi-ingest $HOOT_OPTS -D ogr.reader.node.id.field.name=geonameid $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING SHP REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING SHP REFERENCE LAYER OUTPUT..."
echo ""
hoot diff $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING SHP ADD CHANGESET OUTPUT..."
echo ""
diff $GOLD_ADD_CHANGESET $CHANGESET_ADD_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING SHP DELETE CHANGESET OUTPUT..."
echo ""
diff $GOLD_DELETE_CHANGESET $CHANGESET_DELETE_OUTPUT
echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot db-delete $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"

# Delete the user
PGPASSWORD=$DB_PASSWORD psql $PSQL_DB_AUTH -d $DB_NAME  -c "DELETE FROM users WHERE email='$HOOT_EMAIL';" > /dev/null
