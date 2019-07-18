#!/bin/bash
set -e

# Wholesale Building Replacement Workflow with Strict AOI Handling
#
# This test is not lenient regarding the AOI, in that it will not modify any features in the ref data that lie outside of it. This 
# workflow could work for other polygon or point data types but only buildings have been attempted so far. See related notes in 
# ServiceBuildingReplacementTest.sh

TEST_NAME=ServiceBuildingStrictReplacementTest
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

GENERAL_OPTS="--warn -D log.class.filter=ElementComparer;ChangesetWriter;ChangesetDeriver;SuperfluousNodeRemover;OsmMap -D uuid.helper.repeatable=true -D writer.include.debug.tags=true -D changeset.xml.writer.add.timestamp=false -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D debug.maps.write=true"
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=15 -D perty.systematic.error.y=15 -D perty.ops="
PRUNE_AND_CROP_OPTS="-D convert.ops=hoot::RemoveElementsVisitor -D convert.bounding.box=$AOI -D remove.elements.visitor.element.criteria=hoot::BuildingCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true"
# -D convert.ops=hoot::SuperfluousNodeRemover
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D convert.bounding.box=$AOI -D changeset.reference.keep.entire.features.crossing.bounds=true -D changeset.secondary.keep.entire.features.crossing.bounds=false -D changeset.reference.keep.only.features.inside.bounds=false -D changeset.secondary.keep.only.features.inside.bounds=true"

# DATA PREP

echo ""
echo "Writing the reference dataset to an osm api db (contains features to be replaced)..."
echo ""
hoot convert $GENERAL_OPTS $PERTY_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-ref.osm -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops="hoot::SetTagValueVisitor;hoot::PertyOp" -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.key=name -D set.tag.value.visitor.value="Building 1" $SEC_LAYER_FILE $REF_LAYER_FILE 
scripts/database/CleanAndInitializeOsmApiDb.sh 
hoot convert $GENERAL_OPTS $DB_OPTS -D changeset.user.id=1 -D reader.use.data.source.ids=true $REF_LAYER_FILE $REF_LAYER
echo ""
echo "Writing the secondary dataset to a hoot api db (contains features to replace with)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/data-prep-sec.osm -D reader.use.data.source.ids=true -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.element.criterion=hoot::BuildingCriterion -D set.tag.value.visitor.key=name -D set.tag.value.visitor.value="Building 2" $SEC_LAYER_FILE $SEC_LAYER

# PRUNING AND CROPPING

echo ""
echo "crop and prune"
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS $PRUNE_AND_CROP_OPTS -D debug.maps.filename=$OUT_DIR/prune-and-crop-ref.osm -D reader.use.data.source.ids=true -D convert.bounding.box.keep.entire.features.crossing.bounds=true -D bounds.output.file=$OUT_DIR/$TEST_NAME-bounds.osm $REF_LAYER $OUT_DIR/$TEST_NAME-ref-cropped.osm --write-bounds
hoot convert $GENERAL_OPTS $DB_OPTS $PRUNE_AND_CROP_OPTS -D debug.maps.filename=$OUT_DIR/prune-and-crop-sec.osm -D reader.use.data.source.ids=true -D convert.bounding.box.keep.entire.features.crossing.bounds=false -D convert.bounding.box.keep.only.features.inside.bounds=true $SEC_LAYER $OUT_DIR/$TEST_NAME-sec-cropped.osm

# COOKIE CUTTING

echo ""
echo "cookie cut"
echo ""
hoot generate-alpha-shape $GENERAL_OPTS -D debug.maps.filename=$OUT_DIR/alpha-shape.osm -D reader.use.data.source.ids=true $OUT_DIR/$TEST_NAME-sec-cropped.osm 1000 0 $OUT_DIR/$TEST_NAME-cutter-shape.osm
hoot cookie-cut $GENERAL_OPTS -D debug.maps.filename=$OUT_DIR/cookie-cut.osm -D reader.use.data.source.ids=true -D crop.keep.entire.features.crossing.bounds=true $OUT_DIR/$TEST_NAME-cutter-shape.osm $OUT_DIR/$TEST_NAME-ref-cropped.osm $OUT_DIR/$TEST_NAME-cookie-cut.osm

# CONFLATION

echo ""
echo "conflate"
echo ""
hoot conflate $GENERAL_OPTS -D debug.maps.filename=$OUT_DIR/conflate.osm -D conflate.use.data.source.ids.1=true -D conflate.use.data.source.ids.2=true -D tag.merger.default=hoot::OverwriteTag2Merger $OUT_DIR/$TEST_NAME-cookie-cut.osm $OUT_DIR/$TEST_NAME-sec-cropped.osm $OUT_DIR/$TEST_NAME-conflated.osm

# CHANGESET DERIVATION

CHANGESET_DERIVATION_MSG="Deriving a changeset that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset"
echo ""
echo $CHANGESET_DERIVATION_MSG " (osm xml file secondary source; xml changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-xml.osm $OUT_DIR/$TEST_NAME-ref-cropped.osm $OUT_DIR/$TEST_NAME-conflated.osm $OUT_DIR/$TEST_NAME-changeset-1.osc
echo ""
echo $CHANGESET_DERIVATION_MSG " (osm xml file secondary source; sql changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-sql.osm $OUT_DIR/$TEST_NAME-ref-cropped.osm $OUT_DIR/$TEST_NAME-conflated.osm $OUT_DIR/$TEST_NAME-changeset-2.osc.sql $REF_LAYER

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset to the reference data..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUT_DIR/changeset-apply.osm $OUT_DIR/$TEST_NAME-changeset-2.osc.sql $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUT_DIR/final-write.osm $OSM_API_DB_URL $OUT_DIR/$TEST_NAME-replaced.osm
hoot diff $GENERAL_OPTS $IN_DIR/$TEST_NAME-replaced.osm $OUT_DIR/$TEST_NAME-replaced.osm

# cleanup
hoot delete-db-map -D debug.maps.filename=$OUT_DIR/cleanup.osm $HOOT_OPTS $DB_OPTS $SEC_LAYER
