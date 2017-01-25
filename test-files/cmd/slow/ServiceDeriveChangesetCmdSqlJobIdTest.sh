#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
# We only need to query against the job status table, so no map layer is needed.  HootApiDb 
# requires a map layer, though(hence, 'blah').
export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME/blah"

source scripts/SetupOsmApiDB.sh force
psql --quiet $OSM_API_DB_AUTH -d $DB_NAME_OSMAPI -f test-files/servicesdb/users.sql

rm -rf test-output/cmd/ServiceDeriveChangesetCmdSqlJobIdTest
mkdir -p test-output/cmd/ServiceDeriveChangesetCmdSqlJobIdTest

# generate a dummy job record
JOB_ID="ServiceDeriveChangesetCmdSqlJobIdTest_"$((( RANDOM % 10000 ) + 1))
# echo $JOB_ID
# insert the job record
export INSERT_JOB_RESPONSE=`psql -A -t -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER -c "INSERT INTO job_status (job_id, start, status) VALUES ('$JOB_ID', now(), 2)"`
echo "insert job response: " $INSERT_JOB_RESPONSE

# derive the changeset
hoot derive-changeset test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceDeriveChangesetCmdSqlJobIdTest/changeset.osc.sql $OSM_API_DB_URL $JOB_ID $HOOT_DB_URL
diff test-output/cmd/ServiceDeriveChangesetCmdSqlJobIdTest/changeset.osc.sql test-files/cmd/slow/ServiceDeriveChangesetCmdSqlTest/changeset.osc.sql

# verify the job status detail field contains the changeset file path
export JOB_STATUS_DETAIL=`psql -A -t -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER -c "SELECT status_detail FROM job_status WHERE job_id='$JOB_ID'"`
echo "changeset file: " $JOB_STATUS_DETAIL

