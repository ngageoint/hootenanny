#!/bin/bash
set -e

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep multiary-ingest`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "multiary-ingest command not present.  Exiting test without running it."
 exit 0
fi

EXISTING_INPUT=~/hoot/tmp/geonames/allCountries-11-18-13-100k.geonames
NEW_INPUT=~/hoot/tmp/geonames/allCountries-8-15-17-100k.geonames
REF_DIR=test-files/cmd/slow/MultiaryIngestCmdTest
OUTPUT_DIR=~/hoot/test-output/cmd/slow/MultiaryIngestCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
CHANGESET_OUTPUT=$OUTPUT_DIR/allCountries-changeset.spark.1
FINAL_OUTPUT=$OUTPUT_DIR/allCountries.geonames

source conf/database/DatabaseConfig.sh
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
HOOT_OPTS="--debug -D uuid.helper.repeatable=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org"

hoot delete-map $HOOT_OPTS "$HOOT_DB_URL/ExistingInput"
hoot delete-map $HOOT_OPTS "$HOOT_DB_URL/MultiaryIngest-temp-"
hoot convert $HOOT_OPTS $EXISTING_INPUT "$HOOT_DB_URL/ExistingInput"
hoot multiary-ingest $HOOT_OPTS $NEW_INPUT "$HOOT_DB_URL/ExistingInput" $CHANGESET_OUTPUT true
hoot convert "$HOOT_DB_URL/ExistingInput" $FINAL_OUTPUT
#hoot is-match $REF_DIR/geonames-output.osm $FINAL_OUTPUT 
#diff $REF_DIR/geonames-changeset.spark.1 $CHANGESET_OUTPUT

