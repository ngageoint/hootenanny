#!/bin/bash
set -e

# Wholesale Road Replacement Workflow
#
# This test is lenient regarding the AOI, in that it will modify some features in the ref data that extend outside of it. This workflow 
# could work for other linear data types but only roads have been attempted so far. See related notes in ServiceBuildingReplacementTest.sh

TEST_NAME=ServiceRoadReplacementTest
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

# CONFIG OPTS

GENERAL_OPTS="--warn -D uuid.helper.repeatable=true -D changeset.xml.writer.add.timestamp=false -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false"
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=50 -D perty.systematic.error.y=50 -D perty.ops="
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D convert.bounding.box=-71.4698,42.4866,-71.4657,42.4902 -D convert.ops=hoot::RemoveElementsVisitor;hoot::CookieCutterOp -D remove.elements.visitor.element.criteria=hoot::HighwayCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true -D changeset.reference.keep.entire.features.crossing.bounds=true -D changeset.secondary.keep.entire.features.crossing.bounds=true -D changeset.reference.keep.only.features.inside.bounds=false -D changeset.secondary.keep.only.features.inside.bounds=false"

# DATA PREP

echo ""
echo "Writing the reference dataset to an osm api db (contains features to be replaced)..."
echo ""
hoot convert $GENERAL_OPTS $PERTY_OPTS -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops=hoot::PertyOp $SEC_LAYER_FILE $REF_LAYER_FILE 
scripts/database/CleanAndInitializeOsmApiDb.sh 
hoot convert $GENERAL_OPTS $DB_OPTS -D changeset.user.id=1 -D reader.use.data.source.ids=true $REF_LAYER_FILE $REF_LAYER
# Uncomment to see what the ref layer looks like in file form:
#hoot convert $GENERAL_OPTS $REF_LAYER $OUT_DIR/ref.osm
 
echo ""
echo "Writing the secondary dataset to a hoot api db (contains features to replace with)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=true -D id.generator=hoot::PositiveIdGenerator $SEC_LAYER_FILE $SEC_LAYER
# Uncomment to see what the sec layer looks like in file form:
#hoot convert $GENERAL_OPTS $DB_OPTS $SEC_LAYER $OUT_DIR/sec.osm

# CHANGESET DERIVATION

# TODO: fix strange tag ordering errors on the changeset diffs
# TODO: figure out why changeset-1.osc and changeset-2.osc aren't exactly the same

CHANGESET_DERIVATION_MSG="Deriving a changeset that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset"
echo ""
echo $CHANGESET_DERIVATION_MSG " (osm xml file secondary source; xml changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER_FILE $OUT_DIR/$TEST_NAME-changeset-1.osc
#diff $IN_DIR/$TEST_NAME-changeset-1.osc $OUT_DIR/$TEST_NAME-changeset-1.osc
echo ""
echo $CHANGESET_DERIVATION_MSG " (hoot api db secondary source; xml changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER $OUT_DIR/$TEST_NAME-changeset-2.osc
#diff $IN_DIR/$TEST_NAME-changeset-2.osc $OUT_DIR/$TEST_NAME-changeset-2.osc
echo ""
echo $CHANGESET_DERIVATION_MSG " (osm xml file secondary source; sql changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER_FILE $OUT_DIR/$TEST_NAME-changeset-2.osc.sql $REF_LAYER
#diff $IN_DIR/$TEST_NAME-changeset-2.osc.sql $OUT_DIR/$TEST_NAME-changeset-2.osc.sql

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset to the reference data..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $OUT_DIR/$TEST_NAME-changeset-2.osc.sql $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS $OSM_API_DB_URL $OUT_DIR/$TEST_NAME-replaced.osm
hoot diff $GENERAL_OPTS $IN_DIR/$TEST_NAME-replaced.osm $OUT_DIR/$TEST_NAME-replaced.osm

# cleanup 
hoot delete-db-map $HOOT_OPTS $DB_OPTS $SEC_LAYER
