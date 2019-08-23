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
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

REF_LAYER_FILE=$OUT_DIR/BostonSubsetRoadBuilding_FromOsm-perturbed.osm
REF_LAYER=$OSM_API_DB_URL
SEC_LAYER_FILE=test-files/BostonSubsetRoadBuilding_FromOsm.osm
SEC_LAYER="$HOOT_DB_URL/$TEST_NAME-sec"
AOI="-71.4698,42.4866,-71.4657,42.4902"

GENERAL_OPTS="--warn -D log.class.filter= -D uuid.helper.repeatable=true -D writer.include.debug.tags=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D debug.maps.write=false -D debug.maps.filename=$OUT_DIR/debug.osm"
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D changeset.user.id=1"
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=15 -D perty.systematic.error.y=15 -D perty.ops= "
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D bounds.output.file=$OUT_DIR/$TEST_NAME-bounds.osm"

# DATA PREP

scripts/database/CleanAndInitializeOsmApiDb.sh
echo ""
echo "Writing the reference dataset to an osm api db (contains features to be replaced)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS $PERTY_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref.osm -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops="hoot::SetTagValueVisitor;hoot::PertyOp" -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 1" $SEC_LAYER_FILE $REF_LAYER 
# needed for debugging only:
#hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref.osm -D reader.use.data.source.ids=true $REF_LAYER $REF_LAYER_FILE 
echo ""
echo "Writing the secondary dataset to a hoot api db (contains features to replace with)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-sec.osm -D reader.use.data.source.ids=false -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.keys=name -D set.tag.value.visitor.values="Building 2" $SEC_LAYER_FILE $SEC_LAYER
# needed for debugging only:
#hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-sec.osm -D reader.use.data.source.ids=true $SEC_LAYER $OUT_DIR/$TEST_NAME-sec-layer-original-from-db.osm 

# CHANGESET DERIVATION

echo ""
echo "Deriving a changeset that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset..."
echo ""
hoot changeset-derive-replacement -C UnifyingAlgorithm.conf $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER $AOI $OUT_DIR/$TEST_NAME-changeset-1.osc.sql $REF_LAYER --geometry-filters "hoot::BuildingCriterion" --write-bounds

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset to the reference data..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-apply.osm $OUT_DIR/$TEST_NAME-changeset-1.osc.sql $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/final-write.osm $OSM_API_DB_URL $OUT_DIR/$TEST_NAME-replaced.osm
hoot diff $GENERAL_OPTS $IN_DIR/$TEST_NAME-replaced.osm $OUT_DIR/$TEST_NAME-replaced.osm

# CLEANUP

hoot delete-db-map $HOOT_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/cleanup.osm $SEC_LAYER
