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

# -D log.class.filter=
GENERAL_OPTS="--trace -D log.class.filter=ChangesetDeriver;ElementComparer;WayJoinerAdvanced;UnconnectedWaySnapper;CookieCutConflateWayJoiner;IndexElementsVisitor;OsmUtils;HighwayCriterion -D writer.include.debug.tags=true -D uuid.helper.repeatable=true -D changeset.xml.writer.add.timestamp=false -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D debug.maps.write=true"

DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"

PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=15 -D perty.systematic.error.y=15 -D perty.ops="

# -D cookie.cutter.output.crop=false -D conflate.use.data.source.ids.1=true -D conflate.use.data.source.ids.2=true -D conflate.pre.ops=hoot::RemoveElementsVisitor;hoot::CookieCutConflatorOp

# DATA PREP

echo ""
echo "Writing the reference dataset to an osm api db (contains features to be replaced)..."
echo ""
hoot convert $GENERAL_OPTS $PERTY_OPTS -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops=hoot::PertyOp $SEC_LAYER_FILE $REF_LAYER_FILE 
scripts/database/CleanAndInitializeOsmApiDb.sh 
hoot convert $GENERAL_OPTS $DB_OPTS -D changeset.user.id=1 -D reader.use.data.source.ids=true $REF_LAYER_FILE $REF_LAYER
 
echo ""
echo "Writing the secondary dataset to a hoot api db (contains features to replace with)..."
echo ""
# Add a custom tag to the secondary roads, so we can verify it gets merged into the final output. TODO: add this to the other replacement tests.
# TODO: change other replacement tests to handle secondary IDs in the same way this test does
# -D reader.use.data.source.ids=true -D id.generator=hoot::PositiveIdGenerator
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=false -D convert.ops=hoot::SetTagValueVisitor -D set.tag.value.visitor.element.criterion=hoot::HighwayCriterion -D set.tag.value.visitor.key=replacement_test -D set.tag.value.visitor.value=yes $SEC_LAYER_FILE $SEC_LAYER
# Uncomment to see what the sec layer looks like in file form:
#hoot convert $GENERAL_OPTS $DB_OPTS $SEC_LAYER $OUT_DIR/sec.osm

# CHANGESET DERIVATION
# TODO: rework this whole part with inline map ops, rather than separate commands dumping data out to disk each time

#-D reader.preserve.all.tags=true -D reader.keep.status.tag=true
echo "crop ref"
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=true -D convert.ops=hoot::RemoveElementsVisitor -D convert.bounding.box=$AOI -D convert.bounding.box.keep.entire.features.crossing.bounds=true -D remove.elements.visitor.element.criteria=hoot::HighwayCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true -D debug.maps.filename=$OUT_DIR/ref-read.osm $REF_LAYER $OUT_DIR/01-$TEST_DESCRIPTION-$TEST_NAME-ref-cropped.osm

echo "crop sec"
# -D reader.use.data.source.ids=true 
# Note that convert.bounding.box.keep.entire.features.crossing.bounds=false only works for OSM API data sources when using hoot's 
# OsmApiDbReader, as we doing are here. I don't believe this type of cropping is possible with an http pull via Rails Port or with 
# Overpass...we'd have to implement that cropping behavior custom for them.
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=false -D convert.ops=hoot::RemoveElementsVisitor -D convert.bounding.box=$AOI -D convert.bounding.box.keep.entire.features.crossing.bounds=false -D remove.elements.visitor.element.criteria=hoot::HighwayCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true -D debug.maps.filename=$OUT_DIR/sec-read.osm $SEC_LAYER $OUT_DIR/02-$TEST_DESCRIPTION-$TEST_NAME-sec-cropped.osm

echo "gen alpha shape"
hoot generate-alpha-shape $GENERAL_OPTS -D reader.use.data.source.ids=true -D debug.maps.filename=$OUT_DIR/alpha-shape.osm $OUT_DIR/02-$TEST_DESCRIPTION-$TEST_NAME-sec-cropped.osm 1000 0 $OUT_DIR/03-$TEST_DESCRIPTION-$TEST_NAME-cutter-shape.osm

echo "cookie cut"
#-D cookie.cutter.alpha.shape.buffer=-15.0 -D cookie.cutter.alpha=1000
hoot cookie-cut $GENERAL_OPTS -D reader.use.data.source.ids=true -D debug.maps.filename=$OUT_DIR/cookie-cut.osm $OUT_DIR/03-$TEST_DESCRIPTION-$TEST_NAME-cutter-shape.osm $OUT_DIR/01-$TEST_DESCRIPTION-$TEST_NAME-ref-cropped.osm $OUT_DIR/04-$TEST_DESCRIPTION-$TEST_NAME-cookie-cut.osm 0.0

# TODO: do we want -D tag.merger.default=hoot::AverageTagMerger here, hoot::OverwriteTag1Merger, or something else completely?

echo "conflate"
# -D way.joiner=hoot::CookieCutConflateWayJoiner hoot::WayJoinerAdvanced -D conflate.use.data.source.ids.2=true
hoot conflate $GENERAL_OPTS -D conflate.use.data.source.ids.1=true -D conflate.use.data.source.ids.2=false -D way.joiner=hoot::WayJoinerAdvanced -D tag.merger.default=hoot::OverwriteTag2Merger -D debug.maps.filename=$OUT_DIR/conflate.osm $OUT_DIR/02-$TEST_DESCRIPTION-$TEST_NAME-sec-cropped.osm $OUT_DIR/04-$TEST_DESCRIPTION-$TEST_NAME-cookie-cut.osm $OUT_DIR/05-$TEST_DESCRIPTION-$TEST_NAME-conflated.osm

# TODO: CookieCutConflateWayJoiner doesn't work yet
echo "snap"
# -D way.joiner=hoot::CookieCutConflateWayJoiner hoot::WayJoinerAdvanced
hoot convert $GENERAL_OPTS -D reader.use.data.source.ids=true -D way.joiner=hoot::CookieCutConflateWayJoiner -D tag.merger.default=hoot::OverwriteTag2Merger -D convert.ops="hoot::UnconnectedHighwaySnapper;hoot::WayJoinerOp" -D snap.unconnected.ways.snap.to.way.status=Input2 -D snap.unconnected.ways.snap.way.status=Input1 -D snap.unconnected.ways.existing.way.node.tolerance=10.0 -D snap.unconnected.ways.snap.tolerance=10.0 -D debug.maps.filename=$OUT_DIR/snap.osm $OUT_DIR/05-$TEST_DESCRIPTION-$TEST_NAME-conflated.osm $OUT_DIR/06-$TEST_DESCRIPTION-$TEST_NAME-snapped.osm

#-D reader.use.file.status=true
#-D changeset.allow.deleting.reference.features=false -D changeset.buffer=0.0002

echo "derive xml changeset"
hoot changeset-derive $GENERAL_OPTS -D changeset.user.id=1 -D convert.bounding.box=$AOI -D changeset.reference.keep.entire.features.crossing.bounds=true -D changeset.secondary.keep.entire.features.crossing.bounds=false -D changeset.reference.keep.only.features.inside.bounds=false -D changeset.secondary.keep.only.features.inside.bounds=false $OUT_DIR/01-$TEST_DESCRIPTION-$TEST_NAME-ref-cropped.osm $OUT_DIR/06-$TEST_DESCRIPTION-$TEST_NAME-snapped.osm $OUT_DIR/07-$TEST_DESCRIPTION-$TEST_NAME-changeset-1.osc

echo "derive sql changeset"
hoot changeset-derive $GENERAL_OPTS -D changeset.user.id=1 -D convert.bounding.box=$AOI -D changeset.reference.keep.entire.features.crossing.bounds=true -D changeset.secondary.keep.entire.features.crossing.bounds=false -D changeset.reference.keep.only.features.inside.bounds=false -D changeset.secondary.keep.only.features.inside.bounds=false -D debug.maps.filename=$OUT_DIR/derive-changeset.osm $OUT_DIR/01-$TEST_DESCRIPTION-$TEST_NAME-ref-cropped.osm $OUT_DIR/06-$TEST_DESCRIPTION-$TEST_NAME-snapped.osm $OUT_DIR/$TEST_NAME-changeset-2.osc.sql $REF_LAYER

# CHANGESET APPLICATION

echo ""
echo "Applying the changeset to the reference data..."
echo ""
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $OUT_DIR/$TEST_NAME-changeset-2.osc.sql $OSM_API_DB_URL
echo ""
echo "Reading the entire reference dataset out for verification..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=true $OSM_API_DB_URL $OUT_DIR/08-$TEST_DESCRIPTION-$TEST_NAME-replaced.osm

# cleanup 
hoot delete-db-map $HOOT_OPTS $DB_OPTS $SEC_LAYER
