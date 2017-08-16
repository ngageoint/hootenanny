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

REF_DIR=test-files/cmd/slow/MultiaryIngestCmdTest
OUTPUT_DIR=~/hoot/test-output/cmd/slow/MultiaryIngestCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

source conf/database/DatabaseConfig.sh
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
#--warn
HOOT_OPTS="-D uuid.helper.repeatable=true -D reader.add.source.datetime=false -D writer.include.circular.error.tags=false -D api.db.email=OsmApiDbHootApiDbConflate@hoottestcpp.org -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true"

echo "Writing base data..."
hoot convert $HOOT_OPTS ~/hoot/tmp/geonames/allCountries-11-18-13.geonames "$HOOT_DB_URL/Geonames" #21:41
hoot multiary-ingest $HOOT_OPTS ~/hoot/tmp/geonames/allCountries-8-15-17.geonames "$HOOT_DB_URL/Geonames" $OUTPUT_DIR/allCountries-changeset.spark.1 true
hoot convert "$HOOT_DB_URL/Geonames" $OUTPUT_DIR/allCountries.geonames
#hoot is-match $REF_DIR/geonames-output.osm $OUTPUT_DIR/geonames-output.osm 
#diff $REF_DIR/geonames-changeset.spark.1 $OUTPUT_DIR/geonames-changeset.spark.1

