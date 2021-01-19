#!/bin/bash
set -e

# This tests the changeset output of Reference Conflation within a bounds where the inputs contained 
# fully hydrated relations (API DB queries used automatically hydrate the relations) in the 
# following way:
#
# 1) Data of all types and their parent relations conflated with no input filtering:
#   a) Only features (of any type) that exist within or cross over the bounds should be conflated 
#      and added to the output changeset. No new secondary features should be added that don'
#      satisfy the bounds requirement. 
#   b) Only reference relations who have at least one member that exists within or crosses over the 
#      bounds should be merged with secondary relations and added to the output changeset. Members 
#      in such reference and secondary relations completely outside of the bounds should remain in 
#      their parent relations after changeset application. When two relations are merged, if similar
#      features cannot be matched, there will appear to be duplicated features in the merged 
#      relation. This is expected and the conflicts must be manually resolved.
#   c) Unmerged secondary relations, in or out of bounds, containing features of any type may exist 
#      in the output changeset.
#
# There should not be any relation with incomplete members (members missing) in the output data.
# There should be any duplicated features added to the output. Duplicated features can be difficult 
# to locate without searching for them by name.
#
# The original problem leading to the creation of this test and the other related scenario tests was 
# that because the API DB query was changed at one point to read data in that also includes all 
# parent relations for features within the bounds and subsequently, all the relation members of 
# those relations. This caused two basic problems: 1) relation member features were being conflated 
# that were completely outside of the conflate bounds, which is unexpected and misleading and 2) for 
# datasets with large numbers of long rivers completely outside of the conflate bounds, the 
# increased processing time caused River Conflation to unnecessarily revert to using lesser 
# algorithms to increase runtime performance (see RiverMaximalSublineSettingOptimizer) but yielded 
# lower quality conflate performance. To solve these problems, per feature bounds checking has been
# added to each feature matcher and to changeset derivation.
#
# See other ServiceDiffConflateBounds*Test for other scenarios.

TEST_NAME=ServiceDiffConflateBounds1Test
GOLD_DIR=test-files/cmd/glacial/serial/ServiceDiffConflateBoundsTest
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

GENERAL_OPTS="-C UnifyingAlgorithm.conf -C ReferenceConflation.conf -C Testing.conf -D uuid.helper.repeatable=true -D writer.include.debug.tags=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false"
DB_OPTS="-D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D changeset.user.id=1 -D changeset.max.size=999999" 
BOUNDS="-117.729492166,40.9881915574,-117.718505838,40.996484138672"
# TODO: explain way join options
CONFLATE_OPTS="-D bounds=$BOUNDS -D bounds.output.file=$OUTPUT_DIR/bounds.osm -D waterway.maximal.subline.auto.optimize=true -D way.joiner.write.parent.id.to.child.id=true"
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1 -D changeset.allow.deleting.reference.features=false -D bounds=$BOUNDS"

DEBUG=false
if [ "$DEBUG" == "true" ]; then
  GENERAL_OPTS=$GENERAL_OPTS" -D debug.maps.write=true"
  LOG_LEVEL="--trace"
  LOG_FILTER="-D log.class.filter=OsmApiDbSqlChangesetFileWriter"
fi

scripts/database/CleanAndInitializeOsmApiDb.sh

# write ref to osmapidb
hoot convert $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUTPUT_DIR/ref-load-debug.osm -D reader.use.data.source.ids=true $GOLD_DIR/Input1.osm $OSM_API_DB_URL

# write sec to hootapidb
SEC_INPUT=$HOOT_DB_URL/$TEST_NAME-sec-input
hoot convert $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUTPUT_DIR/sec-load-debug.osm -D reader.use.data.source.ids=true  $GOLD_DIR/Input2.osm $SEC_INPUT

# run ref conflate to osm
CONFLATED=$HOOT_DB_URL/$TEST_NAME-conflated
hoot conflate $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $DB_OPTS $CONFLATE_OPTS -D debug.maps.filename=$OUTPUT_DIR/conflate-debug.osm -D conflate.use.data.source.ids.1=true -D conflate.use.data.source.ids.2=true $OSM_API_DB_URL $SEC_INPUT $CONFLATED --write-bounds

# generate a changeset between the original ref data and the diff calculated in the previous step
hoot changeset-derive $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUTPUT_DIR/changeset-derive-debug.osm  $OSM_API_DB_URL $CONFLATED $OUTPUT_DIR/diff.osc.sql $OSM_API_DB_URL
if [ "$DEBUG" == "true" ]; then
  hoot changeset-derive $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUTPUT_DIR/changeset-derive-debug.osm $OSM_API_DB_URL $CONFLATED $OUTPUT_DIR/diff.osc
fi

# apply changeset back to ref
hoot changeset-apply $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D debug.maps.filename=$OUTPUT_DIR/changeset-apply-debug.osm  $OUTPUT_DIR/diff.osc.sql $OSM_API_DB_URL

# read ref back out and compare to gold
hoot convert $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $DB_OPTS -D debug.maps.filename=$OUTPUT_DIR/final-out-debug.osm -D reader.use.data.source.ids=true $OSM_API_DB_URL $OUTPUT_DIR/out.osm
hoot diff $LOG_LEVEL $LOG_FILTER $GENERAL_OPTS $GOLD_DIR/out-1.osm $OUTPUT_DIR/out.osm

# cleanup
scripts/database/CleanOsmApiDB.sh
hoot db-delete --warn $GENERAL_OPTS $DB_OPTS $SEC_INPUT
hoot db-delete --warn $GENERAL_OPTS $DB_OPTS $CONFLATED
#PGPASSWORD=$DB_PASSWORD psql $PSQL_DB_AUTH -d $DB_NAME -c "DELETE FROM users WHERE email='$HOOT_EMAIL';" > /dev/null
