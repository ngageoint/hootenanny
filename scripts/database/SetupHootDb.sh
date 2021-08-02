#!/usr/bin/env bash

set -e

# Get the configuration for the Database
source $HOOT_HOME/conf/database/DatabaseConfig.sh

# setup Postgresql authentication
export HOOT_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export OSM_AUTH="-h $DB_HOST_OSMAPI -p $DB_PORT_OSMAPI -U $DB_USER_OSMAPI"

# Debug
# echo "### Creating Hoot db ###"
# set | grep "DB_"

pushd /tmp >/dev/null # Stop postgres "could not change directory to" warnings

# Check if we are operating locally or on a remote DB
# If remote, we assume that the owner of the DB has already setup the users
echo "  # Checking for Services Database user \"$DB_USER\" in the postgresql instance on $DB_HOST ###"
if [ "$DB_HOST" = "localhost" ]; then
  # We need to sudo to "postgres" here since we dont know if the user exists
  if ! sudo -u postgres psql -c "\du $DB_USER" | cut -d \| -f 1 | grep -iw --quiet $DB_USER; then
    echo "  # Adding a Services Database user named \"$DB_USER\""
    sudo -u postgres createuser --superuser $DB_USER
    sudo -u postgres psql -c "alter user $DB_USER with password '$DB_PASSWORD';"
  fi
else
  # Use the supplied credentials to query the remote postgres instance
  # NOTE: psql will throw errors when the user or DB doesn't exist. The "|| true" bit stops this
  export PGPASSWORD=$DB_PASSWORD
  ULIST=$(psql $HOOT_AUTH $DB_NAME -c "\du" 2>&1 || true)

# Debug
# echo "-----"
# echo $ULIST
# echo "-----"

  #  User doesn't exist or wrong password: psql: FATAL:  password authentication failed for user "smurf"
  if grep -iw --quiet "authentication failed" <<< $ULIST; then
    echo "ERROR: Hootenanny database user \"$DB_USER\" does not exist or the password is incorrect"
    echo "Please check the database configuration in \$HOOT_HOME/conf/database"
    exit 1
  #  User exists but the datadase doesn't:  psql: error: FATAL:  database "papasmurf" does not exist
  elif grep -iw --quiet "FATAL: database" <<< $ULIST; then
    echo "    - Hootenanny database user \"$DB_USER\" exists"
  #  User and database exist:  Standard Postgresql output for the "\du" command
  elif ! cut -d \| -f 1 <<< $ULIST | grep -iw --quiet $DB_USER; then
    echo "    - Hootenanny database user \"$DB_USER\" exists"
  else
    # Some other output
    echo "ERROR: $ULIST"
    exit 1
  fi
fi

# A bit of funkyness and repeated code to check for the DB_USER_OSMAPI user that is needed
# by the SetupOsmApiDB.sh script
echo "  # Checking for OSM API database user \"$DB_USER_OSMAPI\" in the postgresql instance on $DB_HOST_OSMAPI ###"
if [ "$DB_USER_OSMAPI" = "$DB_USER" ]; then
  # Since the username is the same and we have already checked it, it exists.
  echo "    - OSM API database user \"$DB_USER_OSMAPI\" exists"
else
  if [ "$DB_HOST_OSMAPI" = "localhost" ]; then
    if ! sudo -u postgres psql -c "\du $DB_USER_OSMAPI" | cut -d \| -f 1 | grep -iw --quiet $DB_USER_OSMAPI; then
      echo "  # Adding an OSM API Services Database user named \"$DB_USER_OSMAPI\""
      sudo -u postgres createuser --superuser $DB_USER_OSMAPI
      sudo -u postgres psql -c "alter user $DB_USER_OSMAPI with password '$DB_PASSWORD_OSMAPI';"
    fi
  else
    # Repeat of the code from above. This probably should either be a function or moved out of this file.
    export PGPASSWORD=$DB_PASSWORD_OSMAPI
    ULIST=$(psql $OSM_AUTH $DB_NAME_OSMAPI -c "\du" 2>&1 || true)

    #  User doesn't exist or wrong password: psql: FATAL:  password authentication failed for user "smurf"
    if grep -iw --quiet "authentication failed" <<< $ULIST; then
      echo "ERROR: OSM API database user \"$DB_USER_OSMAPI\" does not exist or the password is incorrect"
      echo "Please check the database configuration in \$HOOT_HOME/conf/database"
      exit 1
    #  User exists but the datadase doesn't:  psql: error: FATAL:  database "papasmurf" does not exist
    elif grep -iw --quiet "FATAL: database" <<< $ULIST; then
      echo "    - OSM API database user \"$DB_USER_OSMAPI\" exists"
    #  User and database exist:  Standard Postgresql output for the "\du" command
    elif ! cut -d \| -f 1 <<< $ULIST | grep -iw --quiet $DB_USER; then
      echo "    - OSM API database user \"$DB_USER_OSMAPI\" exists"
    else
      # Some other output
      echo "ERROR: psql output: $ULIST"
      exit 1
    fi
  fi
fi

# Now get a list of all of the databases
# NOTE: If we passed the user checks, this should not fail
export PGPASSWORD=$DB_PASSWORD
DBLIST=$(psql $HOOT_AUTH -lqt | cut -d \| -f 1)

# Debug
# echo "---"
# echo " DB List"
# echo "$DBLIST"
# echo "---"

# Check for a hoot Db
echo "  # Checking Creating Main Services Database called \"$DB_NAME\" on $DB_HOST ###"
if ! grep -iw --quiet $DB_NAME <<< $DBLIST; then
  echo "    - Creating Main Services Database called \"$DB_NAME\" on $DB_HOST"
  createdb $HOOT_AUTH $DB_NAME --owner=$DB_USER
  psql $HOOT_AUTH -d $DB_NAME -c 'create extension hstore;'

    # TODO: Add the liquibase commands to configure the database.
else
  echo "    - Main Services Database called \"$DB_NAME\" exists"
fi

popd >/dev/null
exit
