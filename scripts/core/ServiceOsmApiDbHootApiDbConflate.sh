#!/bin/bash
set -e

# THIS SCRIPT WAS USED BY THE OLD STYLE HOLY GRAIL TESTS THAT ARE NO LONGER IN USE. ITS KEPT HERE AS REFERENCE ONLY
#
# This is a base test script for conflating datasets where one dataset comes from an osm api database and the other 
# from a hoot api database.  It simulates end to end at the command line level what one possible conflation workflow 
# with MapEdit data looks like (aka Holy Grail).  See ServiceOsmApiDbHootApiDbAllDataTypesConflateTest.sh for an 
# example of how to call this script.
# 
# This script:
#   - writes two datasets, one to an OSM API database and one to a Hoot API database; assumes the two have overlapping aoi's 
#   - reads in the same subset AOI from each dataset
#   - conflates the data together and writes the result to a Hoot API database
#   - writes out a sql changeset file that is the difference between the original OSM API database reference dataset and the conflated output in the Hoot API database
#   - executes the changeset file SQL against the OSM API database
#   - reads out the entire contents of the OSM API database and verifies it

# If you pass "generate-random" in place of an actual AOI, then tiles-calculate will be used to randomly generate 
# usable AOI's, one of which will be selected.  If you pass "generate-random;<integer>" in place of an actual AOI, then 
# the behavior is the same as previous described except the random AOI selection is seeded for reproducible results.  
# This feature is useful in exploring potential issues using this workflow with various datasets.

# If we add xml changeset apply capabilities, then this script will need to take in a test type param and run twice
# for each test, once for sql and once for xml OR create two completely separate scripts, one for sql changeset derivation 
# and one for xml changeset derivation.

REF_DATASET=$1
SEC_DATASET=$2
AOI=$3 # leave blank to select a random AOI
#AOI=-180,-90,180,90 # for debugging
#'unifying' or 'network'
CONFLATION_TYPE=$4
TEST_NAME=$5
TEST_CATEGORY=$6
SELECT_RANDOM_AOI=$7
RANDOM_SEED=$8

echo "reference dataset: " $REF_DATASET
echo "secondary dataset: " $SEC_DATASET
echo "AOI: " $AOI
echo "CONFLATION TYPE: " $CONFLATION_TYPE
echo "TEST_NAME: " $TEST_NAME
echo "TEST_CATEGORY: " $TEST_CATEGORY
echo "SELECT_RANDOM_AOI: " $SELECT_RANDOM_AOI
echo "RANDOM_SEED: " $RANDOM_SEED

RUN_DEBUG_STEPS=false

# set to false for testing only
LOAD_REF_DATA=true
LOAD_SEC_DATA=true
CONFLATE_DATA=true
HOOT_EMAIL=OsmApiDbHootApiDbConflate@hoottestcpp.org

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
# generic debugging options applicable to multiple commands
HOOT_OPTS="--warn -D uuid.helper.repeatable=true -D writer.include.debug.tags=true"
if [ "$CONFLATION_TYPE" == "network" ]; then
  HOOT_OPTS=$HOOT_OPTS" -D match.creators=hoot::NetworkMatchCreator -D merger.creators=hoot::NetworkMergerCreator -D network.matcher=hoot::ConflictsNetworkMatcher -D conflate.match.highway.classifier=hoot::HighwayExpertClassifier -D way.subline.matcher=hoot::MaximalSublineMatcher -D rubber.sheet.minimum.ties=4 -D rubber.sheet.ref=true"
fi

REF_DIR=test-files/cmd/$TEST_CATEGORY/$TEST_NAME
OUTPUT_DIR=test-output/cmd/$TEST_CATEGORY/$TEST_NAME
rm -rf $OUTPUT_DIR/*
mkdir -p $OUTPUT_DIR

if [ "$SELECT_RANDOM_AOI" == "true" ]; then
  echo ""
  echo "STEP 0: Calculating tiles and selecting one at random as the conflate AOI..."
  echo ""
  hoot tiles-calculate-random $HOOT_OPTS "$REF_DATASET;$SEC_DATASET" $OUTPUT_DIR/tile.geojson $RANDOM_SEED 10000 0.001
  AOI=`hoot extent --error $OUTPUT_DIR/tile.geojson false`
  echo "AOI: " $AOI
fi

if [ "$LOAD_REF_DATA" == "true" ]; then
  echo ""
  echo "STEP 1: Cleaning out the osm api db and initializing it for use..."
  echo ""
  scripts/database/CleanAndInitializeOsmApiDb.sh

  echo ""
  echo "STEP 2: Writing the complete reference dataset to the osm api db..."
  echo ""
  if [[ $REF_DATASET == *"pbf"* ]]; then
    #we want this in .osm format to be able to examine it for debugging purposes anyway
    hoot convert $HOOT_OPTS $REF_DATASET $OUTPUT_DIR/2-ref-raw-complete.osm
  else
    cp $REF_DATASET $OUTPUT_DIR/2-ref-raw-complete.osm
  fi 
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D changeset.user.id=1 -D apidb.bulk.inserter.validate.data=true -D osmapidb.bulk.inserter.reserve.record.ids.before.writing.data=true -D apidb.bulk.inserter.output.files.copy.location=$OUTPUT_DIR/2-ref-raw-complete.sql $OUTPUT_DIR/2-ref-raw-complete.osm $OSM_API_DB_URL
fi

if [ "$RUN_DEBUG_STEPS" == "true" ]; then
  echo ""
  echo "STEP 3: Reading the complete reference dataset out of the osm api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D writer.include.circular.error.tags=false $OSM_API_DB_URL $OUTPUT_DIR/3-ref-complete-PulledFromOsmApiDb.osm
fi

if [ "$RUN_DEBUG_STEPS" == "true" ]; then
  echo ""
  echo "STEP 4: Reading the subset AOI reference dataset out of the osm api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D convert.bounding.box=$AOI -D writer.include.circular.error.tags=false $OSM_API_DB_URL $OUTPUT_DIR/4-ref-subset-PulledFromOsmApiDb.osm
fi

if [ "$LOAD_SEC_DATA" == "true" ]; then
  echo ""
  echo "STEP 5: Writing the complete secondary dataset to the hoot api db..."
  echo ""
  if [[ $SEC_DATASET == *"pbf"* ]]; then
    #we want this in .osm format to be able to examine it for debugging purposes anyway
    hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true $SEC_DATASET $OUTPUT_DIR/5-secondary-raw-complete.osm
  else
    cp $SEC_DATASET $OUTPUT_DIR/5-secondary-raw-complete.osm
  fi
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true $OUTPUT_DIR/5-secondary-raw-complete.osm "$HOOT_DB_URL/5-secondary-complete-$TEST_NAME"
fi

if [ "$RUN_DEBUG_STEPS" == "true" ]; then
  echo ""
  echo "STEP 6: Reading the complete secondary dataset out of the hoot api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true "$HOOT_DB_URL/5-secondary-complete-$TEST_NAME" $OUTPUT_DIR/6-secondary-complete-PulledFromHootApiDb.osm
fi

if [ "$RUN_DEBUG_STEPS" == "true" ]; then
  echo ""
  echo "STEP 7: Reading the subset AOI secondary dataset out of the hoot api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D convert.bounding.box=$AOI "$HOOT_DB_URL/5-secondary-complete-$TEST_NAME" $OUTPUT_DIR/7-secondary-subset-PulledFromHootApiDb.osm
fi

if [ "$CONFLATE_DATA" == "true" ]; then
  # We're writing the output to the hoot api db first here, rather than directly to the osm api db, b/c if there are reviews 
  # we want to give the user a chance to review them.  That can only happen when the output is stored in a hoot api db.

  echo ""
  echo "STEP 8a: Conflating the two datasets over the specified AOI with the SQL changeset workflow..."
  echo ""
  hoot conflate $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D api.db.email=$HOOT_EMAIL -D convert.bounding.box=$AOI -D conflate.use.data.source.ids.2=true -D osm.map.reader.factory.reader=hoot::OsmApiDbAwareHootApiDbReader -D osm.map.writer.factory.writer=hoot::OsmApiDbAwareHootApiDbWriter -D osmapidb.id.aware.url=$OSM_API_DB_URL $OSM_API_DB_URL "$HOOT_DB_URL/5-secondary-complete-$TEST_NAME" "$HOOT_DB_URL/8a-conflated-$TEST_NAME"

  echo ""
  echo "STEP 8b: Conflating the two datasets over the specified AOI with the XML changeset workflow..."
  echo ""
  # How is this working w/o conflate.use.data.source.ids.2=true? How can changeset derivation work if not all IDs are preserved?
  hoot conflate $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D api.db.email=$HOOT_EMAIL -D convert.bounding.box=$AOI -D id.generator=hoot::PositiveIdGenerator -D osm.map.writer.factory.writer=hoot::NonIdRemappingHootApiDbWriter $OSM_API_DB_URL "$HOOT_DB_URL/5-secondary-complete-$TEST_NAME" "$HOOT_DB_URL/8b-conflated-$TEST_NAME"
fi

if [ "$RUN_DEBUG_STEPS" == "true" ]; then
  echo ""
  echo "STEP 9a: Reading the complete conflated dataset with the SQL changeset workflow out of the hoot api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D api.db.email=$HOOT_EMAIL -D writer.include.circular.error.tags=false "$HOOT_DB_URL/8a-conflated-$TEST_NAME" $OUTPUT_DIR/9a-conflated-complete-PulledFromHootApiDb.osm

  echo ""
  echo "STEP 9b: Reading the complete conflated dataset with the XML changeset workflow out of the hoot api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D api.db.email=$HOOT_EMAIL -D writer.include.circular.error.tags=false "$HOOT_DB_URL/8b-conflated-$TEST_NAME" $OUTPUT_DIR/9b-conflated-complete-PulledFromHootApiDb.osm
fi

if [ "$RUN_DEBUG_STEPS" == "true" ]; then
  echo ""
  echo "STEP 10a: Reading the subset AOI conflated dataset with the SQL changeset workflow out of the hoot api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D api.db.email=$HOOT_EMAIL -D writer.include.circular.error.tags=false "$HOOT_DB_URL/8a-conflated-$TEST_NAME" $OUTPUT_DIR/10a-conflated-subset-PulledFromHootApiDb.osm

  echo ""
  echo "STEP 10b: Reading the subset AOI conflated dataset with the XML changeset workflow out of the hoot api db and writing it into a file (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D api.db.email=$HOOT_EMAIL -D writer.include.circular.error.tags=false "$HOOT_DB_URL/8b-conflated-$TEST_NAME" $OUTPUT_DIR/10b-conflated-subset-PulledFromHootApiDb.osm
fi

echo ""
echo "STEP 11a: Writing a SQL changeset file that is the difference between the cropped reference input dataset specified AOI and the conflated output specified AOI..."
echo ""
hoot changeset-derive $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D reader.use.file.status=true -D reader.keep.status.tag=true -D changeset.user.id=1 -D convert.bounding.box=$AOI -D changeset.buffer=0.001 -D changeset.allow.deleting.reference.features=false $OSM_API_DB_URL "$HOOT_DB_URL/8a-conflated-$TEST_NAME" $OUTPUT_DIR/11a-conflated-changeset-ToBeAppliedToOsmApiDb.osc.sql $OSM_API_DB_URL

echo ""
echo "STEP 11b: Writing an XML changeset file that is the difference between the cropped reference input dataset specified AOI and the conflated output specified AOI..."
echo ""
# changeset.xml.writer.add.timestamp should only be set false for this test's purposes; leave it set to the default value of
# true for production purposes
hoot changeset-derive $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D reader.use.file.status=true -D reader.keep.status.tag=true -D changeset.user.id=1 -D convert.bounding.box=$AOI -D changeset.buffer=0.001 -D changeset.allow.deleting.reference.features=false -D changeset.xml.writer.add.timestamp=false $OSM_API_DB_URL "$HOOT_DB_URL/8b-conflated-$TEST_NAME" $OUTPUT_DIR/11b-conflated-changeset-ToBeAppliedToOsmApiDb.osc

echo ""
echo "STEP 12: Executing the SQL changeset on the osm api db..."
echo ""
hoot changeset-apply $HOOT_OPTS $OUTPUT_DIR/11a-conflated-changeset-ToBeAppliedToOsmApiDb.osc.sql $OSM_API_DB_URL

# no way to apply xml changesets without a rails port instance in place

if [ "$RUN_DEBUG_STEPS" == "true" ]; then
  echo ""
  echo "STEP 13: Reading the contents of the osm api db for the specified aoi, for the SQL changeset workflow, writing it into a file, and verifying it (debug)..."
  echo ""
  hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D writer.include.circular.error.tags=false -D convert.bounding.box=$AOI $OSM_API_DB_URL $OUTPUT_DIR/13-subset-output-PulledFromOsmApiDb.osm
fi

echo ""
echo "STEP 14: Reading the entire contents of the osm api db, for the SQL changeset workflow, writing it into a file, and verifying the data..."
echo ""
hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D writer.include.circular.error.tags=false $OSM_API_DB_URL $OUTPUT_DIR/14-complete-output-PulledFromOsmApiDb.osm
hoot diff $HOOT_OPTS $REF_DIR/output.osm $OUTPUT_DIR/14-complete-output-PulledFromOsmApiDb.osm

# The map comparison, step 14, should be enough to check the state of the changeset write.  Due to node coordinate
# precision differences between Ubuntu and CentOS, we'd need the equivalent of the diff command for changesets
# to do a valid changeset comparison.

#echo ""
#echo "STEP 15a: Verifying the SQL changeset..."
#echo ""
#diff $REF_DIR/output.osc.sql $OUTPUT_DIR/11a-conflated-changeset-ToBeAppliedToOsmApiDb.osc.sql

#echo ""
#echo "STEP 15b: Verifying the XML changeset..."
#echo ""
#diff $REF_DIR/output.osc $OUTPUT_DIR/11b-conflated-changeset-ToBeAppliedToOsmApiDb.osc

#
echo ""
echo "STEP 15: Cleaning up database..."
echo ""
if [ "$LOAD_SEC_DATA" == "true" ]; then
  hoot db-delete-map -D api.db.email=$HOOT_EMAIL "$HOOT_DB_URL/5-secondary-complete-$TEST_NAME"
fi

if [ "$CONFLATE_DATA" == "true" ]; then
  hoot db-delete-map -D api.db.email=$HOOT_EMAIL "$HOOT_DB_URL/8a-conflated-$TEST_NAME"
  hoot db-delete-map -D api.db.email=$HOOT_EMAIL "$HOOT_DB_URL/8b-conflated-$TEST_NAME"
fi
