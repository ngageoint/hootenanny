#!/bin/bash
set -e

# This test creates a diff between two road datasets and snaps unconnected secondary roads back to reference roads before the changeset output
# with just the secondary data is created. It then writes the changeset back to the reference data store after which we may check to see that 
# all the snapped secondary roads remain connected to reference roads.

TEST_NAME=ServiceDiffRoadSnapTest
INPUT_DIR=test-files/cmd/slow/DiffConflateCmdTest
GOLD_DIR=test-files/cmd/glacial/serial/$TEST_NAME
OUTPUT_DIR=test-output/cmd/glacial/serial/$TEST_NAME
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

GENERAL_OPTS="--debug -C DifferentialConflation.conf -C NetworkAlgorithm.conf -C Testing.conf -D log.class.filter= -D uuid.helper.repeatable=true -D writer.include.debug.tags=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D debug.maps.write=false"
DB_OPTS="-D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D changeset.user.id=1"
CHANGESET_DERIVE_OPTS="-D changeset.user.id=1"

scripts/database/CleanAndInitializeOsmApiDb.sh

# write ref to osmapidb
hoot convert $GENERAL_OPTS $DB_OPTS $INPUT_DIR/input3.osm $OSM_API_DB_URL

# write sec to hootapidb
hoot convert $GENERAL_OPTS $DB_OPTS $INPUT_DIR/input4.osm $HOOT_DB_URL/$TEST_NAME-sec

# run diff conflate to changeset w/ road snapping - can only directly apply the .osc.sql file, so that's required, but let's output the .osc as 
# well, as its easier to read during debugging
hoot conflate $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D differential.snap.unconnected.roads=true -D snap.unconnected.ways.snap.tolerance=10.0 $OSM_API_DB_URL $HOOT_DB_URL/DiffHgTest-sec $OUTPUT_DIR/diff.osc.sql $OSM_API_DB_URL --differential
hoot conflate $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS -D differential.snap.unconnected.roads=true -D snap.unconnected.ways.snap.tolerance=10.0 $OSM_API_DB_URL $HOOT_DB_URL/DiffHgTest-sec $OUTPUT_DIR/diff.osc --differential

# apply changeset back to ref
hoot changeset-apply $GENERAL_OPTS $DB_OPTS $CHANGESET_DERIVE_OPTS $OUTPUT_DIR/diff.osc.sql $OSM_API_DB_URL

# read ref back out
hoot convert $OSM_API_DB_URL $OUTPUT_DIR/out.osm

# compare to gold
hoot diff $GOLD_DIR/out.osm $OUTPUT_DIR/out.osm

# cleanup
hoot db-delete-map $HOOT_OPTS $DB_OPTS -$SEC_DB_INPUT $HOOT_DB_URL/$TEST_NAME-sec


