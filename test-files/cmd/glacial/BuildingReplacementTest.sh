#!/bin/bash
set -e

# Wholesale Building Replacement Workflow
#
# This could work for other data types but only buildings are being focused on initially. This workflow:
#
#   - Distorts one layer with perturbation and loads it into an OSM API DB as ref data (the perturbed buildings are uglier, so we'd want to get rid them)
#   - Reads the unperturbed data from either a file or into a Hoot API DB as secondary data (unperturbed buildings looks good, so let's keep)
#   - Cuts an AOI out of ref data and puts the secondary data into that location
#   - Derives a changeset that is the difference between the unmodified ref data and the data with the section of new data added to it
#   - The resultant changeset should have deleted all ref data within the AOI and added all the secondary data within it.

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

GENERAL_OPTS="--info -D uuid.helper.repeatable=true -D writer.include.debug.tags=true "
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=1 -D perty.systematic.error.y=1"
# I've chosen to leave conflation (hoot::UnifyingConflator) out as the last convert op, since it hasn't been needed yet to make the building
# output look good...it may be needed at some point, though, and would likely be needed with features like roads.
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D convert.bounding.box=-71.4698,42.4866,-71.4657,42.4902 -D convert.ops=hoot::RemoveElementsVisitor;hoot::CookieCutterOp -D remove.elements.visitor.element.criteria=hoot::BuildingCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true"

# Commented out here are settings to tweak tag reading/writing behavior:
#-D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D writer.include.circular.error.tags=false

# Commented out here are settings to tweak cookie cutting behavior:
#-D cookie.cutter.alpha=1000 -D cookie.cutter.alpha.shape.buffer=0.0

# Commented out here are settings to tweak changeset derivation behavior:
#-D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D changeset.xml.writer.add.timestamp=false -D changeset.allow.deleting.reference.features=true

# DATA PREP

echo ""
echo "Writing the reference dataset to an osm api db (contains buildings to be replaced)..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh 
# Preserving the source data IDs (reader.use.data.source.ids=true) is important here for changeset derivation since this is the ref data. 
# (I forget why changeset.user.id would be needed here)
hoot convert $GENERAL_OPTS $DB_OPTS $PERTY_OPTS -D changeset.user.id=1 -D reader.use.data.source.ids=true -D convert.ops=hoot::PertyOp $REF_LAYER_FILE $REF_LAYER
# Uncomment to see what the ref layer looks like in file form:
# hoot convert $GENERAL_OPTS $REF_LAYER $OUT_DIR/ref.osm

echo ""
echo "Writing the secondary dataset to a hoot api db (contains buildings to replace with)..."
echo ""
# Since the perturbed data (ref) was derived directly from this secondary layer, there will overlapping element IDs. Changeset derivation with
# cookie cutting (or any map consuming op) cannot work when there are overlapping element IDs in the two input datasets...we simply can't support
# that. So, we'll pretend these secondary buildings are from a completely different source than the ref and drop their IDs 
# (reader.use.data.source.ids=false).
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=false $SEC_LAYER_FILE $SEC_LAYER
# uncomment to see what the sec layer looks like in file form:
# hoot convert $GENERAL_OPTS $DB_OPTS $SEC_LAYER $OUT_DIR/sec.osm

# END DATA PREP

# TODO: this won't work correctly until bounds is supported for .osm inputs
#echo ""
#echo "Deriving a changeset that completely replaces buildings in the reference dataset within the specified AOI with those from a secondary dataset (osm xml file secondary source)..."
#echo ""
#hoot changeset-derive $GENERAL_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER test-files/BostonSubsetRoadBuilding_FromOsm.osm $OUT_DIR/boston-cookie-cut-out-1.osc

echo ""
echo "Deriving a changeset that completely replaces buildings in the reference dataset within the specified AOI with those from a secondary dataset (hoot api db secondary source)..."
echo ""
hoot changeset-derive $HGENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER $OUT_DIR/BuildingReplacementTest-out-2.osc

# CLEANUP

hoot delete-db-map $HOOT_OPTS $DB_OPTS $SEC_LAYER
