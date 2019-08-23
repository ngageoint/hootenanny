#!/bin/bash
set -e

# Wholesale Building Replacement Workflow
#
# This test is lenient regarding the AOI, in that it may modify some features in the ref data that lie just outside of the AOI. Secondary 
# features crossing the AOI may be included in the output or conflated with reference features. This workflow should work other polygon 
# data types, but only buildings have been tested with it so far.

TEST_NAME=ServiceChangesetReplacementBuildingTest
IN_DIR=test-files/cmd/glacial/serial/$TEST_NAME
OUT_DIR=test-output/cmd/glacial/serial/$TEST_NAME
rm -rf $OUT_DIR
mkdir -p $OUT_DIR

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI

SOURCE_FILE=test-files/BostonSubsetRoadBuilding_FromOsm.osm
AOI="-71.4698,42.4866,-71.4657,42.4902"

GENERAL_OPTS="--warn -D log.class.filter= -D uuid.helper.repeatable=true -D writer.include.debug.tags=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D debug.maps.write=false"
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D changeset.user.id=1"
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=15 -D perty.systematic.error.y=15 -D perty.ops= "
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D bounds.output.file=$OUT_DIR/$TEST_NAME-bounds.osm"

# DB SOURCE WORKFLOW

HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
REF_DB_INPUT=$OSM_API_DB_URL
SEC_DB_INPUT="$HOOT_DB_URL/$TEST_NAME-sec"
CHANGESET_DB=$OUT_DIR/$TEST_NAME-changeset-db.osc.sql
OUT_DB=$TEST_NAME-db-replaced.osm

# DATA PREP

echo ""
echo "Writing the reference dataset from: $SOURCE_FILE to an osm api db: $REF_DB_INPUT (contains features to be replaced)..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $GENERAL_OPTS $DB_OPTS $PERTY_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref-db.osm -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops="hoot::SetTagValueVisitor;hoot::PertyOp" -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 1" $SOURCE_FILE $REF_DB_INPUT 
# needed for debugging only:
#hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref.osm -D reader.use.data.source.ids=true $REF_DB_INPUT $OUT_DIR/ref-original-tmp.osm
echo ""
echo "Writing the secondary dataset from: $SOURCE_FILE to a hoot api db: $SEC_DB_INPUT (contains features to replace with)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-sec-db.osm -D reader.use.data.source.ids=false -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 2" $SOURCE_FILE $SEC_DB_INPUT
# needed for debugging only:
#hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-sec.osm -D reader.use.data.source.ids=true $SEC_DB_INPUT $OUT_DIR/sec-original-tmp.osm

# CHANGESET DERIVATION

echo ""
echo "Deriving a changeset between $REF_DB_INPUT and $SEC_DB_INPUT to file: $CHANGESET_DB that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset..."
echo ""
hoot changeset-derive-replacement -C UnifyingAlgorithm.conf $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-derive-db.osm $REF_DB_INPUT $SEC_DB_INPUT $AOI $CHANGESET_DB $REF_DB_INPUT --geometry-filters "hoot::BuildingCriterion" --write-bounds

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset: $CHANGESET_DB to the reference data in: $OSM_API_DB_URL..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-apply-db.osm $CHANGESET_DB $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out of: $OSM_API_DB_URL to: $OUT_DB for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/final-write-db.osm $OSM_API_DB_URL $OUT_DIR/$OUT_DB
hoot diff $GENERAL_OPTS $IN_DIR/$OUT_DB $OUT_DIR/$OUT_DB

# CLEANUP

hoot delete-db-map $HOOT_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/cleanup-db.osm $SEC_DB_INPUT

# XML SOURCE WORKFLOW

REF_XML_INPUT=$OUT_DIR/ref-perturbed-original.osm
SEC_XML_INPUT=$OUT_DIR/sec-original.osm
CHANGESET_XML=$OUT_DIR/$TEST_NAME-changeset-xml.osc.sql
OUT_XML=$TEST_NAME-xml-replaced.osm

# DATA PREP

echo ""
echo "Writing the reference dataset from: $SOURCE_FILE to an osm api db: $REF_DB_INPUT (contains features to be replaced)..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $GENERAL_OPTS $DB_OPTS $PERTY_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref-xml.osm -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops="hoot::SetTagValueVisitor;hoot::PertyOp" -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 1" $SOURCE_FILE $REF_DB_INPUT
echo ""
echo "Writing the reference dataset from: $REF_DB_INPUT to an xml file: $REF_XML_INPUT (contains features to be replaced)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref-xml.osm -D reader.use.data.source.ids=true $REF_DB_INPUT $REF_XML_INPUT
echo ""
echo "Writing the secondary dataset from: $SOURCE_FILE to an xml file: $SEC_XML_INPUT (contains features to replace with)..."
echo ""
hoot convert $GENERAL_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-sec-xml.osm -D reader.use.data.source.ids=false -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 2" $SOURCE_FILE $SEC_XML_INPUT

# CHANGESET DERIVATION

echo ""
echo "Deriving a changeset between $REF_XML_INPUT and $SEC_XML_INPUT to file: $CHANGESET_XML that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset..."
echo ""
hoot changeset-derive-replacement -C UnifyingAlgorithm.conf $GENERAL_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-derive-xml.osm $REF_XML_INPUT $SEC_XML_INPUT $AOI $CHANGESET_XML $REF_DB_INPUT --geometry-filters "hoot::BuildingCriterion"

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset: $CHANGESET_XML to the reference data in: $OSM_API_DB_URL..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-apply-xml.osm $CHANGESET_XML $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out of: $OSM_API_DB_URL to: $OUT_XML for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/final-write-xml.osm $OSM_API_DB_URL $OUT_DIR/$OUT_XML
hoot diff $GENERAL_OPTS $IN_DIR/$OUT_XML $OUT_DIR/$OUT_XML

# JSON SOURCE WORKFLOW

REF_JSON_INPUT=$OUT_DIR/ref-perturbed-original.json
SEC_JSON_INPUT=$OUT_DIR/sec-original.json
CHANGESET_JSON=$OUT_DIR/$TEST_NAME-changeset-json.osc.sql
OUT_JSON=$TEST_NAME-json-replaced.osm

# DATA PREP

echo ""
echo "Writing the reference dataset from: $SOURCE_FILE to an osm api db: $REF_DB_INPUT (contains features to be replaced)..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
hoot convert $GENERAL_OPTS $DB_OPTS $PERTY_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref-json.osm -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops="hoot::SetTagValueVisitor;hoot::PertyOp" -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 1" $SOURCE_FILE $REF_DB_INPUT
echo ""
echo "Writing the reference dataset from: $REF_DB_INPUT to a json file: $REF_JSON_INPUT (contains features to be replaced)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref-json.osm -D reader.use.data.source.ids=true $REF_DB_INPUT $REF_JSON_INPUT
echo ""
echo "Writing the secondary dataset from: $SOURCE_FILE to a json file: $SEC_JSON_INPUT (contains features to replace with)..."
echo ""
hoot convert $GENERAL_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-sec-json.osm -D reader.use.data.source.ids=false -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 2" $SOURCE_FILE $SEC_JSON_INPUT

# CHANGESET DERIVATION

echo ""
echo "Deriving a changeset between $REF_JSON_INPUT and $SEC_JSON_INPUT to file: $CHANGESET_JSON that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset..."
echo ""
hoot changeset-derive-replacement -C UnifyingAlgorithm.conf $GENERAL_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-derive-json.osm $REF_JSON_INPUT $SEC_JSON_INPUT $AOI $CHANGESET_JSON $REF_DB_INPUT --geometry-filters "hoot::BuildingCriterion"

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset: $CHANGESET_JSON to the reference data in: $OSM_API_DB_URL..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-apply-json.osm $CHANGESET_JSON $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out of: $OSM_API_DB_URL to: $OUT_JSON for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/final-write-json.osm $OSM_API_DB_URL $OUT_DIR/$OUT_JSON
hoot diff $GENERAL_OPTS $IN_DIR/$OUT_XML $OUT_DIR/$OUT_JSON
