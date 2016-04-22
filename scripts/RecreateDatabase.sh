#!/bin/bash

source $HOOT_HOME/conf/DatabaseConfig.sh

export PGPASSWORD=$DB_PASSWORD
sudo -u postgres dropdb $DB_NAME -p $DB_PORT
sudo -u postgres createdb $DB_NAME --owner=$DB_USER -p $DB_PORT
sudo -u postgres psql $DB_NAME -c 'create extension hstore;' -p $DB_PORT

