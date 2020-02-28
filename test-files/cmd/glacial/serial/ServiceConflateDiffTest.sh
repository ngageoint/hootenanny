#!/bin/bash
set -e

# Test conflates two inputs and generates a changeset between the reference and the conflated output. The point of the test is to ensure there
# are no more create/delete statements generated than what there should be.

CONFIG="--warn -C Testing.conf -D writer.include.debug.tags=true -D uuid.helper.repeatable=true -D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true -D changeset.user.id=1"

TEST_NAME=ServiceConflateDiffTest
IN_DIR=/home/vagrant/hoot/tmp/3851
OUT_DIR=$IN_DIR

# create the osm api db
source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
REF_DB_INPUT=$OSM_API_DB_URL
scripts/database/CleanAndInitializeOsmApiDb.sh

# load the osm api db with ref
REF_FILE=$IN_DIR/NOME_40429e.osm
hoot convert $CONFIG $REF_FILE $OSM_API_DB_URL
# pull the ref out for viewing
hoot convert $OSM_API_DB_URL $OUT_DIR/ref.osm

# load the hoot api db with sec
SEC_FILE=$IN_DIR/OSM_40429e.osm 
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
hoot convert $CONFIG $SEC_FILE "$HOOT_DB_URL/$TEST_NAME-sec"
# pull the sec out for viewing
hoot convert "$HOOT_DB_URL/$TEST_NAME-sec" $OUT_DIR/sec.osm

# conflate ref with sec into a new hoot api db layer
hoot conflate $CONFIG $OSM_API_DB_URL "$HOOT_DB_URL/$TEST_NAME-sec" "$HOOT_DB_URL/$TEST_NAME-sec-conflated"
# pull the conflated out for viewing
hoot convert "$HOOT_DB_URL/$TEST_NAME-sec-conflated" $OUT_DIR/conflated.osm

# derive a changeset between ref and conflated; derive the sql version b/c that's what we must apply
hoot changeset-derive $CONFIG $OSM_API_DB_URL "$HOOT_DB_URL/$TEST_NAME-sec-conflated" $OUT_DIR/diff.osc.sql
# derive an xml version for viewing
hoot changeset-derive $CONFIG $OSM_API_DB_URL "$HOOT_DB_URL/$TEST_NAME-sec-conflated" $OUT_DIR/diff.osc

# apply the changeset to the ref
hoot changeset-apply $CONFIG $OUT_DIR/diff.osc.sql $OSM_API_DB_URL

# read the updated ref back out
hoot convert $CONFIG $OSM_API_DB_URL $OUT_DIR/changeset-applied.osm
