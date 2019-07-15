#!/bin/bash
set -e

# Wholesale Road Replacement Workflow with Strict AOI Handling
#
# This test is not lenient regarding the AOI, in that it will not modify any features in the ref data that extend outside of it. This 
# workflow could work for other linear data types but only roads have been attempted so far. See related notes in 
# ServiceBuildingReplacementTest.sh

TEST_NAME=ServiceRoadStrictReplacementTest
IN_DIR=test-files/cmd/glacial/serial/$TEST_NAME
OUT_DIR=test-output/cmd/glacial/serial/$TEST_NAME
TEST_DESCRIPTION=cookwayjoin
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

# CONFIG OPTS

# -D log.class.filter=ChangesetDeriver;ElementComparer;WayJoinerAdvanced;UnconnectedWaySnapper;CookieCutConflateWayJoiner;OsmUtils;IndexElementsVisitor
GENERAL_OPTS="--warn -D log.class.filter= -D writer.include.debug.tags=true -D uuid.helper.repeatable=true -D changeset.xml.writer.add.timestamp=false -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D debug.maps.write=false -D debug.maps.filename=$OUT_DIR/debug.osm"
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=15 -D perty.systematic.error.y=15 -D perty.ops="
# We are deriving the changeset by looking at all parts of the reference features, regardless of whether they cross the AOI bounds, and 
# we are looking at the secondary features in the same way. Its counterintuitive that we wouldn't only look at the secondary features inside
# of the AOI bounds to calculate the changeset, but the changeset won't be created properly if we do it in that manner (TODO: need to explain 
# why).
# problem areas that will be encountered when converting changeset derivation to a single command:
# -D reader.use.data.source.ids=true/false -D way.joiner=hoot::WayJoinerAdvanced
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D convert.bounding.box=$AOI -D convert.ops=hoot::RemoveElementsVisitor;hoot::CookieCutterOp;hoot::UnifyingConflator;hoot::UnconnectedHighwaySnapper;hoot::WayJoinerOp -D remove.elements.visitor.element.criteria=hoot::HighwayCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true -D conflate.use.data.source.ids.1=true -D conflate.use.data.source.ids.2=false -D tag.merger.default=hoot::OverwriteTag2Merger -D way.joiner=hoot::CookieCutConflateWayJoiner -D snap.unconnected.ways.snap.to.way.status=Input2 -D snap.unconnected.ways.snap.way.status=Input1;Conflated -D snap.unconnected.ways.existing.way.node.tolerance=50.0 -D snap.unconnected.ways.snap.tolerance=50.0 -D changeset.reference.keep.entire.features.crossing.bounds=true -D changeset.secondary.keep.entire.features.crossing.bounds=true -D changeset.reference.keep.only.features.inside.bounds=false -D changeset.secondary.keep.only.features.inside.bounds=false"

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
# Add a custom tag to the secondary roads, so we can verify it gets merged into the final output. TODO: add this feature to the other 
# replacement tests.
# TODO: change other replacement tests to handle secondary IDs in the same way this test does
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=false -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.element.criterion=hoot::HighwayCriterion -D set.tag.value.visitor.key=replacement_test -D set.tag.value.visitor.value=yes $SEC_LAYER_FILE $SEC_LAYER
# Uncomment to see what the sec layer looks like in file form:
#hoot convert $GENERAL_OPTS $DB_OPTS $SEC_LAYER $OUT_DIR/sec.osm

# CHANGESET DERIVATION

# Even though the sql changeset is what's applied, the xml changeset is also written so it can be viewed in JOSM.
CHANGESET_DERIVATION_MSG="Deriving a changeset that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset"
echo ""
echo $CHANGESET_DERIVATION_MSG " (hoot api db secondary source; xml changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER $OUT_DIR/$TEST_NAME-changeset-1.osc
#diff $IN_DIR/$TEST_NAME-changeset-2.osc $OUT_DIR/$TEST_NAME-changeset-1.osc
echo ""
echo $CHANGESET_DERIVATION_MSG " (osm xml file secondary source; sql changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER_FILE $OUT_DIR/$TEST_NAME-changeset-1.osc.sql $REF_LAYER
#diff $IN_DIR/$TEST_NAME-changeset-2.osc.sql $OUT_DIR/$TEST_NAME-changeset-1.osc.sql

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset to the reference data..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $OUT_DIR/$TEST_NAME-changeset-1.osc.sql $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=true $OSM_API_DB_URL $OUT_DIR/$TEST_NAME-replaced.osm
hoot diff $GENERAL_OPTS $IN_DIR/output.osm $OUT_DIR/$TEST_NAME-replaced.osm

# CLEANUP 

hoot delete-db-map $HOOT_OPTS $DB_OPTS $SEC_LAYER
