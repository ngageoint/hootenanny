#!/bin/bash
set -e

# Wholesale Building Replacement Workflow
#
#   - loads one layer into an OSM API DB as ref
#   - reads a second from either a file or Hoot API DB as secondary
#   - cuts an AOI out of ref and puts the secondary data in that location

OUT_DIR=test-output/cmd/glacial/BuildingReplacementTest
rm -rf $OUT_DIR
mkdir -p $OUT_DIR

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

REF_LAYER_FILE=test-files/BostonSubsetRoadBuilding_FromOsm-perturbed.osm
REF_LAYER=$OSM_API_DB_URL
SEC_LAYER_FILE=test-files/BostonSubsetRoadBuilding_FromOsm.osm
SEC_LAYER="$HOOT_DB_URL/BuildingReplacementTest-sec"
AOI="-71.4698,42.4866,-71.4657,42.4902"

GENERAL_OPTS="--info -D uuid.helper.repeatable=true -D writer.include.debug.tags=true "
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=1 -D perty.systematic.error.y=1"
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1  -D convert.ops=hoot::RemoveElementsVisitor;hoot::CookieCutterOp -D remove.elements.visitor.element.criteria=hoot::BuildingCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true"

# Commented out here are settings to tag reading/writing behavior:
# -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D writer.include.circular.error.tags=false

# Commented out here are settings to tweak cookie cutting behavior:
# -D cookie.cutter.alpha=1000 -D cookie.cutter.alpha.shape.buffer=0.0

# Commented out here are settings to tweak changeset derivation behavior:
# -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D changeset.xml.writer.add.timestamp=false -D changeset.allow.deleting.reference.features=true

# DATA PREP

# reader.use.data.source.ids

echo ""
echo "Writing the reference dataset to an osm api db (contains buildings to be replaced)..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh 
# using perturbed buildings as the original ref data here, b/c they're uglier (we'd want to get rid them)
hoot convert $GENERAL_OPTS $DB_OPTS $PERTY_OPTS -D apidb.bulk.inserter.validate.data=true -D osmapidb.bulk.inserter.reserve.record.ids.before.writing.data=true -D convert.ops=hoot::PertyOp -D changeset.user.id=1 $REF_LAYER_FILE $REF_LAYER
# Uncomment to see what the ref looks like in file form:
# hoot convert $GENERAL_OPTS $REF_LAYER $OUT_DIR/ref.osm

echo ""
echo "Writing the secondary dataset to a hoot api db (contains buildings to replace with)..."
echo ""
# using unperturbed buildings as the secondary replacement buildings here, bc they're prettier (we'd want to keep them)
hoot convert $GENERAL_OPTS $DB_OPTS $SEC_LAYER_FILE $SEC_LAYER
# uncomment to see what the sec looks like in file form:
# hoot convert $GENERAL_OPTS $DB_OPTS $SEC_LAYER $OUT_DIR/sec.osm

# END DATA PREP

# TODO: this won't work correctly until bounds is supported for .osm inputs
#echo ""
#echo "Deriving a changeset that completely replaces buildings in the reference dataset within the specified AOI with those from a secondary dataset (osm xml file secondary source)..."
#echo ""
#hoot changeset-derive $GENERAL_OPTS $CHANGESET_DERIVE_OPTS -D convert.bounding.box.osm.api.database=$AOI $REF_LAYER test-files/BostonSubsetRoadBuilding_FromOsm.osm $OUT_DIR/boston-cookie-cut-out-1.osc

echo ""
echo "Deriving a changeset that completely replaces buildings in the reference dataset within the specified AOI with those from a secondary dataset (hoot api db secondary source)..."
echo ""
# TODO: do we need conflation too?
hoot changeset-derive $HGENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D convert.bounding.box=$AOI $REF_LAYER $SEC_LAYER $OUT_DIR/BuildingReplacementTest-out-2.osc

# CLEANUP

hoot delete-db-map $HOOT_OPTS $DB_OPTS $SEC_LAYER
