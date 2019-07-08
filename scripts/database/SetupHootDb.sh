#!/usr/bin/env bash

set -e

# Get the configuration for the Database
source $HOOT_HOME/conf/database/DatabaseConfig.sh

# setup db, user, and password to avoid password prompt
export AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD

# Debug
# echo "### Creating Hoot db ###"
# set | grep "DB_"

pushd /tmp >/dev/null # Stop postgres "could not change directory to" warnings

# Check if we are operating locally or on a remote DB
# If remote, we assume that the owner of the DB has already setup the users
echo "  # Checking for Services Database user $DB_USER in the postgresql instance on $DB_HOST ###"
if [ "$DB_HOST" = "localhost" ]; then
  # We need to sudo to "postgres" here since we dont know if the user exists
  if ! sudo -u postgres psql -c "\du" | cut -d \| -f 1 | grep -iw --quiet $DB_USER; then
    echo "  # Adding a Services Database user named $DB_USER"
    sudo -u postgres createuser --superuser $DB_USER
    sudo -u postgres psql -c "alter user $DB_USER with password '$DB_PASSWORD';"
  fi
else
  # Use the supplied credentials to query the remote postgres instance
  # If the command fails we should get something like this: 
  #     psql: FATAL:  password authentication failed for user "smurf"
  # If we don't get a failure, the credentials are good.
  if ! psql $AUTH -c "\du" | cut -d \| -f 1 | grep -iw --quiet $DB_USER; then
    echo "Please check the database configuration in \$HOOT_HOME/conf/database"
    exit 1
  fi
fi

# A bit of funkyness and repeated code to check for the DB_USER_OSMAPI user that is needed
# by the SetupOsmApiDb.sh script
echo "  # Checking for OSM API database user $DB_USER_OSMAPI in the postgresql instance on $DB_HOST ###"
if [ "$DB_USER_OSMAPI" != "$DB_USER" ]; then
  if ! psql $AUTH -c "\du" | cut -d \| -f 1 | grep -iw --quiet $DB_USER_OSMAPI; then
    if [ "$DB_HOST" = "localhost" ]; then
      echo "  # Adding a Services Database user named $DB_USER"
      sudo -u postgres createuser --superuser $DB_USER_OSMAPI
      sudo -u postgres psql -c "alter user $DB_USER_OSMAPI with password '$DB_PASSWORD_OSMAPI';"
    else
      echo "ERROR: OSM API database user $DB_USER_OSMAPI does not exist"
      echo "Please check the database configuration in \$HOOT_HOME/conf/database"
      exit 1
    fi
  fi
fi

# Now get a list of all of the databases
# NOTE: If we passed the user checks, this should not fail
DBLIST=`psql $AUTH -lqt | cut -d \| -f 1`

# Debug
# echo "---"
# echo " DB List"
# echo "$DBLIST"
# echo "---"

# Check for a hoot Db
if ! grep -iw --quiet $DB_NAME <<< $DBLIST; then
    echo "  # Creating Main Services Database called $DB_NAME on $DB_HOST"
    createdb $AUTH $DB_NAME --owner=$DB_USER
    psql $AUTH -d $DB_NAME -c 'create extension hstore;'

    # TODO: Add the liquibase commands to configure the database.
fi

popd >/dev/null
exit
