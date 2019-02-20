#!/bin/bash
set -e

source $HOOT_HOME/conf/database/DatabaseConfig.sh
export PGPASSWORD=$DB_PASSWORD
psql -h $DB_HOST -p $DB_PORT -d $DB_NAME -U $DB_USER -f $HOOT_HOME/scripts/database/add_karma_test_user.sql
