#!/bin/bash
set -e

# TODO: update description to include non-river specific issues also fixed

# This tests River Reference Conflation within a bounding box. The original problem leading to the 
# creation of this test was that the API DB query was changed at one point to read data in that also 
# includes all parent relations for rivers within the bounds and subsequently, all the relation 
# members of those relations. This caused two basic problems: 1) rivers were being conflated that 
# were completely outside of the conflate bounds, which is misleading and 2) for datasets with 
# large numbers of long rivers completely outside of the conflate bounds, the increased processing 
# time caused River Conflation to unnecessarily revert to using lesser algorithms to increase 
# runtime performance (see #RiverMaximalSublineSettingOptimizer). Extra per feature bounds checking 
# has been added to river match candidate checking within River.js to prevent this. After that 
# change, the conflate portion of this test runs quickly and properly merges rivers within the 
# bounds.

TEST_NAME=ServiceRefConflateChangesetRiverTest
GOLD_DIR=test-files/cmd/glacial/serial/$TEST_NAME
OUTPUT_DIR=test-output/cmd/glacial/serial/$TEST_NAME
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export PSQL_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
HOOT_EMAIL="$TEST_NAME@hoottestcpp.org"
LOG_LEVEL="--warn"
LOG_FILTER=""

# -D bounds.remove.missing.elements=false -D debug.maps.remove.missing.elements=false
GENERAL_OPTS="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf -C Testing.conf -D uuid.helper.repeatable=true -D writer.include.debug.tags=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false"
DB_OPTS="-D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D changeset.user.id=1 -D changeset.max.size=999999" 
# -D match.creators=hoot::ScriptMatchCreator,River.js;hoot::ScriptMatchCreator,Relation.js -D merger.creators=hoot::ScriptMergerCreator;hoot::ScriptMergerCreator
CONFLATE_OPTS="-D match.creators=hoot::ScriptMatchCreator,River.js;;hoot::ScriptMatchCreator,Relation.js -D merger.creators=hoot::ScriptMergerCreator;hoot::ScriptMergerCreator -D bounds=-117.729492166,40.9881915574,-117.718505838,40.996484138672 -D bounds.output.file=$OUTPUT_DIR/bounds.osm -D waterway.maximal.subline.auto.optimize=true"
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D changeset.allow.deleting.reference.features=false -D bounds=-117.729492166,40.9881915574,-117.718505838,40.996484138672"

DEBUG=false
if [ "$DEBUG" == "true" ]; then
  GENERAL_OPTS=$GENERAL_OPTS" -D debug.maps.write=true"
  LOG_LEVEL="--trace"
  LOG_FILTER="-D log.class.filter=Relation.js;CopyMapSubsetOp;RelationMerger;InBoundsCriterion "
fi

scripts/database/CleanAndInitializeOsmApiDb.sh

# write ref to osmapidb
hoot convert $LOG_LEVEL $LOG_FILTER -D debug.maps.filename=$OUTPUT_DIR/ref-load-debug.osm -D reader.use.data.source.ids=true $GENERAL_OPTS $DB_OPTS $GOLD_DIR/Input1.osm $OSM_API_DB_URL

# write sec to hootapidb
SEC_INPUT=$HOOT_DB_URL/$TEST_NAME-sec-input
hoot convert $LOG_LEVEL $LOG_FILTER -D debug.maps.filename=$OUTPUT_DIR/sec-load-debug.osm -D reader.use.data.source.ids=true $GENERAL_OPTS $DB_OPTS $GOLD_DIR/Input2.osm $SEC_INPUT

# run ref conflate to osm
SEC_CONFLATED=$HOOT_DB_URL/$TEST_NAME-sec-conflated
hoot conflate $LOG_LEVEL $LOG_FILTER -D debug.maps.filename=$OUTPUT_DIR/conflate-debug.osm $GENERAL_OPTS $DB_OPTS $CONFLATE_OPTS -D conflate.use.data.source.ids.1=true -D conflate.use.data.source.ids.2=true $OSM_API_DB_URL $SEC_INPUT $SEC_CONFLATED --write-bounds

# generate a changeset between the original ref data and the diff calculated in the previous step
hoot changeset-derive $LOG_LEVEL $LOG_FILTER -D debug.maps.filename=$OUTPUT_DIR/changeset-derive-debug.osm $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $OSM_API_DB_URL $SEC_CONFLATED $OUTPUT_DIR/diff.osc.sql $OSM_API_DB_URL
if [ "$DEBUG" == "true" ]; then
  hoot changeset-derive $LOG_LEVEL $LOG_FILTER -D debug.maps.filename=$OUTPUT_DIR/changeset-derive-debug.osm $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $OSM_API_DB_URL $SEC_CONFLATED $OUTPUT_DIR/diff.osc
fi

# apply changeset back to ref
hoot changeset-apply $LOG_LEVEL $LOG_FILTER -D debug.maps.filename=$OUTPUT_DIR/changeset-apply-debug.osm $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $OUTPUT_DIR/diff.osc.sql $OSM_API_DB_URL

# read ref back out and compare to gold
hoot convert $LOG_LEVEL $LOG_FILTER -D debug.maps.filename=$OUTPUT_DIR/final-out-debug.osm $GENERAL_OPTS $DB_OPTS -D reader.use.data.source.ids=true $OSM_API_DB_URL $OUTPUT_DIR/out.osm
hoot diff $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $GOLD_DIR/out.osm $OUTPUT_DIR/out.osm

hoot db-delete --warn $GENERAL_OPTS $DB_OPTS $SEC_DB_INPUT $SEC_INPUT
hoot db-delete --warn $GENERAL_OPTS $DB_OPTS $SEC_DB_INPUT $SEC_CONFLATED

# Delete the user
PGPASSWORD=$DB_PASSWORD psql $PSQL_DB_AUTH -d $DB_NAME -c "DELETE FROM users WHERE email='$HOOT_EMAIL';" > /dev/null
