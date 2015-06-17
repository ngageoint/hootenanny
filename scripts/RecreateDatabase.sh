#!/bin/bash

source $HOOT_HOME/conf/DatabaseConfig.sh

export PGPASSWORD=$DB_PASSWORD
sudo -u postgres dropdb $DB_NAME
sudo -u postgres createdb $DB_NAME --owner=$DB_USER
sudo -u postgres psql $DB_NAME -c 'create extension hstore;'

