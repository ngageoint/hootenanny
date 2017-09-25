#!/bin/bash
set -e

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep multiary-ingest`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "multiary-ingest command not present.  Exiting test without running it."
 exit 0
fi

REF_DIR=test-files/cmd/slow/ServiceMultiaryIngestCmdTest
OUTPUT_DIR=test-output/cmd/slow/ServiceMultiaryIngestCmdTest

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

source conf/database/DatabaseConfig.sh
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
# set the max elements per map in such a way that the partial reading occurs
HOOT_OPTS="--warn -D uuid.helper.repeatable=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D max.elements.per.partial.map=2"

GOLD_OUTPUT=$REF_DIR/allCountries-geonames-output.osm
GOLD_CHANGESET=$REF_DIR/allCountries-geonames-changeset.spark.1

# GEONAMES

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.geonames
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.geonames
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-geonames-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-geonames-changeset.spark.1
TRANSLATION_SCRIPT=translations/GeoNames.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot delete-map $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING GEONAMES REFERENCE DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING GEONAMES NEW DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING GEONAMES REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING GEONAMES REFERENCE LAYER OUTPUT..."
echo ""
hoot is-match $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING GEONAMES CHANGESET OUTPUT..."
echo ""
diff $GOLD_CHANGESET $CHANGESET_OUTPUT

GOLD_OUTPUT=$REF_DIR/allCountries-osmpbf-output.osm
GOLD_CHANGESET=$REF_DIR/allCountries-osmpbf-changeset.spark.1

# OSM PBF

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.osm.pbf
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.osm.pbf
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-osmpbf-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-osmpbf-changeset.spark.1
TRANSLATION_SCRIPT=translations/OSM_Ingest.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot delete-map $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING OSM PBF REFERENCE DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING OSM PBF NEW DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING OSM PBF REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING OSM PBF REFERENCE LAYER OUTPUT..."
echo ""
hoot is-match $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING OSM PBF CHANGESET OUTPUT..."
echo ""
diff $GOLD_CHANGESET $CHANGESET_OUTPUT

GOLD_OUTPUT=$REF_DIR/allCountries-osmxml-output.osm
GOLD_CHANGESET=$REF_DIR/allCountries-osmxml-changeset.spark.1

# OSM XML

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.osm
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.osm
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-osmxml-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-osmxml-changeset.spark.1
TRANSLATION_SCRIPT=translations/OSM_Ingest.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot delete-map $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING OSM XML REFERENCE DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING OSM XML NEW DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING OSM XML REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING OSM XML REFERENCE LAYER OUTPUT..."
echo ""
hoot is-match $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING OSM XML CHANGESET OUTPUT..."
echo ""
diff $GOLD_CHANGESET $CHANGESET_OUTPUT

GOLD_OUTPUT=$REF_DIR/allCountries-shp-output.osm
GOLD_CHANGESET=$REF_DIR/allCountries-shp-changeset.spark.1

# SHP

REFERENCE_INPUT=$REF_DIR/allCountries-11-18-13-10.shp/Points.shp
NEW_INPUT=$REF_DIR/allCountries-8-15-17-10.shp/Points.shp
FINAL_OUTPUT=$OUTPUT_DIR/allCountries-shp-output.osm
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-shp-changeset.spark.1
TRANSLATION_SCRIPT=translations/OSM_Ingest.js

echo ""
echo "MULTIARY INGEST - DELETING REFERENCE LAYER..."
echo ""
hoot delete-map $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer"
echo ""
echo "MULTIARY INGEST - INGESTING SHP REFERENCE DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS -D ogr.reader.node.id.field.name=geonameid $REFERENCE_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - INGESTING SHP NEW DATASET..."
echo ""
hoot multiary-ingest $HOOT_OPTS -D ogr.reader.node.id.field.name=geonameid $NEW_INPUT $TRANSLATION_SCRIPT "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $CHANGESET_OUTPUT
echo ""
echo "MULTIARY INGEST - EXPORTING SHP REFERENCE LAYER..."
echo ""
hoot convert $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-ReferenceLayer" $FINAL_OUTPUT
echo ""
echo "MULTIARY INGEST - COMPARING SHP REFERENCE LAYER OUTPUT..."
echo ""
hoot is-match $HOOT_OPTS $GOLD_OUTPUT $FINAL_OUTPUT 
echo ""
echo "MULTIARY INGEST - COMPARING SHP CHANGESET OUTPUT..."
echo ""
diff $GOLD_CHANGESET $CHANGESET_OUTPUT

