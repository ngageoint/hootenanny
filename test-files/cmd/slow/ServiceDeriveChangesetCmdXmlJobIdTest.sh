#!/bin/bash
set -e

source $HOOT_HOME/conf/DatabaseConfig.sh
# We only need to query against the job status table, so no map layer is needed.  HootApiDb 
# requires a map layer, though (hence, 'blah').
export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME/blah"

mkdir -p test-output/cmd/ServiceDeriveChangesetCmdXmlJobIdTest
rm -f test-output/cmd/ServiceDeriveChangesetCmdXmlJobIdTest/changeset.osc

# insert a dummy job record
JOB_ID="ServiceDeriveChangesetCmdXmlJobIdTest_"$((( RANDOM % 10000 ) + 1))
# echo $JOB_ID
export INSERT_JOB_RESPONSE=`psql -A -t -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER -c "INSERT INTO job_status (job_id, start, status) VALUES ('$JOB_ID', now(), 2)"`
echo "insert job response: " $INSERT_JOB_RESPONSE

# derive the changeset
hoot derive-changeset test-files/cmd/quick/DeriveChangesetCmdTest/map1.osm test-files/cmd/quick/DeriveChangesetCmdTest/map2.osm test-output/cmd/ServiceDeriveChangesetCmdXmlJobIdTest/changeset.osc $JOB_ID $HOOT_DB_URL
diff test-output/cmd/ServiceDeriveChangesetCmdXmlJobIdTest/changeset.osc test-files/cmd/quick/DeriveChangesetCmdTest/changeset.osc

# verify the job status detail field contains the changeset file path
export JOB_STATUS_DETAIL=`psql -A -t -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER -c "SELECT status_detail FROM job_status WHERE job_id='$JOB_ID'"`
echo "changeset file: " $JOB_STATUS_DETAIL
