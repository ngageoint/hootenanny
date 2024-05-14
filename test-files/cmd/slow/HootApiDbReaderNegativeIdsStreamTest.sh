#!/bin/bash
set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh

DB_EMAIL="HootApiDbReaderNegativeIdsStream@hoot.com"
DB_OPT="-D hootapi.db.writer.create.user=true -D api.db.email=$DB_EMAIL -D hootapi.db.writer.overwrite.map=true"
DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
DATABASE="$DB_URL/HootApiDbReaderNegativeIdsStreamTest"

DB_TRANSLATE="-D schema.translation.script=test-files/jakarta_raya_coastline.js"
OUT_TRANSLATE="-D schema.translation.script=$HOOT_HOME/translations/TDSv71.js"

INPUT_FILE="$HOOT_HOME/test-files/jakarta_raya_coastline.shp"
OUTPUT_PATH="$HOOT_HOME/test-output/cmd/slow/HootApiDbReaderNegativeIdsStream"
OUTPUT_FILE_SINGLE="$OUTPUT_PATH/output_single.gdb"
OUTPUT_FILE_MULTI="$OUTPUT_PATH/output_multi.gdb"

LOG_LEVEL="--info"
CONFIG="-C Testing.conf"

# Delete previous output
rm -rf $OUTPUT_PATH
mkdir -p $OUTPUT_PATH

echo
echo "Convert and push to database"
hoot convert $LOG_LEVEL $CONFIG \
 $DB_TRANSLATE \
 $DB_OPT \
 $INPUT_FILE \
 $DATABASE

echo
echo "Get data from database and convert to GDB - Multithreaded"
rm -rf $OUTPUT_FILE_MULTI
hoot convert $LOG_LEVEL $CONFIG \
 $OUT_TRANSLATE \
 $DB_OPT \
 -D writer.thematic.structure=true \
 -D convert.translate.multithreaded=true \
 $DATABASE \
 $OUTPUT_FILE_MULTI
 
echo
echo "Get data from database and convert to GDB - Single threaded"
rm -rf $OUTPUT_FILE_SINGLE
hoot convert $LOG_LEVEL $CONFIG \
 $OUT_TRANSLATE \
 $DB_OPT \
 -D writer.thematic.structure=true \
 -D convert.translate.multithreaded=false \
 $DATABASE \
 $OUTPUT_FILE_SINGLE

echo
echo "Cleanup database"
hoot db-delete $LOG_LEVEL $CONFIG -D api.db.email=$DB_EMAIL $DATABASE
# Delete the user
PGPASSWORD=$DB_PASSWORD psql -h $DB_HOST -p $DB_PORT -U $DB_USER $PSQL_DB_AUTH -d $DB_NAME -c "DELETE FROM users WHERE email='${DB_EMAIL}';" > /dev/null

