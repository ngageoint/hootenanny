#!/bin/bash
set -e

# Wholesale Building Replacement Workflow
#
# This could work for other data types but only buildings have been attempted so far. The main takeaway from this test if you're trying
# to do your own similar workflow is the changeset derivation itself...ends up looking something like:
#
# hoot changeset-derive -D changeset.xml.writer.add.timestamp=false -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D changeset.user.id=1 -D convert.bounding.box=-71.4698,42.4866,-71.4657,42.4902 -D convert.ops=hoot::RemoveElementsVisitor;hoot::CookieCutterOp -D remove.elements.visitor.element.criteria=hoot::BuildingCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true ref.osm sec.osm changeset.osc

TEST_NAME=ServiceBuildingReplacementTest
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

# opts to apply to all the commands; 
# - changeset.xml.writer.add.timestamp=false and reader.add.source.datetime=false are for testing purposes primarily so that the simple 
# diff between gold and output changesets works (we don't have a map diff for changesets).
# - writer.include.circular.error.tags=false simply keeps the output cleaner for changeset derivation
GENERAL_OPTS="--warn -D uuid.helper.repeatable=true -D changeset.xml.writer.add.timestamp=false -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false"
# opts to apply to commands involved in db i/o
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"
# perturbation opts; We just want a small amount of noticeable shift in the features and don't want to apply any of the other non-shift 
# destructive perty ops.
PERTY_OPTS="-D perty.seed=1 -D perty.systematic.error.x=15 -D perty.systematic.error.y=15 -D perty.ops="
# changeset derivation opts
# - I've chosen to leave conflation (hoot::UnifyingConflator) out as the last op in convert.ops, since it hasn't been needed yet to make 
# the building output look good...it may be needed at some point, though, and would likely be needed with features like roads. 
# - The RemoveElementsVisitor is set up to keep only buildings.
# - The behavior of API DB readers when using convert.bounding.box is to return features that cross the bounds in addition to those within it,
# so you will see features slightly passed the bounds modified by the changeset in this workflow. To restrict the changes to just features
# falling completely within the bounds would require some significant work to the ApiDbReader bounds query. Bounds reading by the OsmXmlReader,
# however, could currently support both of the scenarios but it is hardcoded to behave in the same fashion as the db readers for now.
# TODO: comment on rop.keep.entire.features.crossing.bounds
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D convert.bounding.box=-71.4698,42.4866,-71.4657,42.4902 -D convert.ops=hoot::RemoveElementsVisitor;hoot::CookieCutterOp -D remove.elements.visitor.element.criteria=hoot::BuildingCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true -D convert.bounding.box.keep.entire.features.crossing.bounds=true -D convert.bounding.box.keep.only.features.inside.bounds=false"

# Additional config opts that may end up being useful for: 

# debugging:
#-D writer.include.debug.tags=true

# tweaking tag reading/writing behavior:
#-D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true

# tweaking cookie cutting behavior:
#-D cookie.cutter.alpha=1000 -D cookie.cutter.alpha.shape.buffer=0.0

# tweaking changeset derivation behavior:
#-D changeset.allow.deleting.reference.features=true -D changeset.buffer=0.0

# DATA PREP

# First distort one layer with perturbation and loads it into an OSM API DB as ref data. Perturbation was done only b/c I couldn't find readily
# available two similar but not identical test building datasets. The perturbed features are uglier, so the idea is we'd want to get them
# replaced by something better (what we have in the original, unperturbed data). 
#
# Preserving the source data IDs (reader.use.data.source.ids=true in the second convert step) is important here for changeset derivation since 
# we're loading ref data that we don't own (like MapEdit) and need to stay in sync with. 

# The reason we're loading the perturbed data into a file before loading it into the osm apidb, vs peturbing it straight to the osm api db is
# that the test data has negative IDs and we want to simulate a realistic scenario with postive IDs in the ref (hence, 
# hoot::PositiveIdGenerator).
echo ""
echo "Writing the reference dataset to an osm api db (contains features to be replaced)..."
echo ""
hoot convert $GENERAL_OPTS $PERTY_OPTS -D reader.use.data.source.ids=false -D id.generator=hoot::PositiveIdGenerator -D convert.ops=hoot::PertyOp $SEC_LAYER_FILE $REF_LAYER_FILE 
scripts/database/CleanAndInitializeOsmApiDb.sh 
hoot convert $GENERAL_OPTS $DB_OPTS -D changeset.user.id=1 -D reader.use.data.source.ids=true $REF_LAYER_FILE $REF_LAYER
# Uncomment to see what the ref layer looks like in file form:
#hoot convert $GENERAL_OPTS $REF_LAYER $OUT_DIR/ref.osm

# Next, read the original unperturbed data from a file into a Hoot API DB as secondary data. The unperturbed features looks good, so we'll want 
# to keep them). 

# Since the perturbed data (ref) was derived directly from this secondary layer, there will overlapping element IDs. Changeset 
# derivation with cookie cutting (or any map consuming op) cannot work when there are overlapping element IDs in the two input datasets...we 
# simply can't support that scenario. TODO: This does get a little confusing as I think we should be able to use either 
# reader.use.data.source.ids equal to true or false here (with false making more sense), however errors are occurring when its set to false, 
# so leaving it at true for now. We do, however, want positive IDs put into the hoot api db since negative IDs in the db make no sense. 
echo ""
echo "Writing the secondary dataset to a hoot api db (contains features to replace with)..."
echo ""
hoot convert $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=true -D id.generator=hoot::PositiveIdGenerator $SEC_LAYER_FILE $SEC_LAYER
# TODO: For some strange reason this file output data is corrupted unless which layers IDs get element IDs preserved gets swapped
# as described in the comments. So swap the values of reader.use.data.source.ids in the two convert commands that load db data if you want 
# to see this output..will figure out what's going on  later.
# Uncomment to see what the sec layer looks like in file form:
#hoot convert $GENERAL_OPTS $DB_OPTS $SEC_LAYER $OUT_DIR/sec.osm

# CHANGESET DERIVATION

# Test changeset derivation both from an osm file and from a hoot api db layer. 

# This changeset derivation has cookie cutting applied to it which cuts a subset AOI out of ref data and puts the secondary data into 
# that location. Then, it derives a changeset that is the difference between the unmodified ref data and the data with the section of 
# new data added to it. The resultant changeset should have deleted all ref data within the AOI and added all the secondary data within it.
CHANGESET_DERIVATION_MSG="Deriving a changeset that completely replaces features in the reference dataset within the specified AOI with those from a secondary dataset"
echo ""
echo $CHANGESET_DERIVATION_MSG " (osm xml file secondary source; xml changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER_FILE $OUT_DIR/$TEST_NAME-changeset-1.osc
diff $IN_DIR/$TEST_NAME-changeset-1.osc $OUT_DIR/$TEST_NAME-changeset-1.osc
echo ""
echo $CHANGESET_DERIVATION_MSG " (hoot api db secondary source; xml changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER $OUT_DIR/$TEST_NAME-changeset-2.osc
diff $IN_DIR/$TEST_NAME-changeset-2.osc $OUT_DIR/$TEST_NAME-changeset-2.osc
# Go ahead and spit out a sql changeset in the same manner the XML changeset was generated...we'll need it for the next step.
echo ""
echo $CHANGESET_DERIVATION_MSG " (hoot api db secondary source; sql changeset out)..."
echo ""
hoot changeset-derive $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $REF_LAYER $SEC_LAYER $OUT_DIR/$TEST_NAME-changeset-2.osc.sql $REF_LAYER
diff $IN_DIR/$TEST_NAME-changeset-2.osc.sql $OUT_DIR/$TEST_NAME-changeset-2.osc.sql

# CHANGESET APPLICATION

# Write the SQL changeset back to the ref db. We're using the SQL changeset here instead of the XML b/c that's the only kind we can write 
# w/o a Rails Port in the hoot stack (which, if added, would only overly complicate the hoot testing environment)...the effect is the same.
# The final output should have the better looking (unperturbed) features inserted only in the AOI of the ref map (there will be some overlap
# of the AOI bounds). The rest of the map should have perturbed features remaining in it.
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
#scripts/database/CleanOsmApiDB.sh 
hoot delete-db-map $HOOT_OPTS $DB_OPTS $SEC_LAYER
