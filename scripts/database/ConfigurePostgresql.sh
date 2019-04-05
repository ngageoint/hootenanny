#!/usr/bin/env bash

set -e

# Need to figure out a way to do this automagically
#PG_VERSION=$(sudo -u postgres psql -c 'SHOW SERVER_VERSION;' | egrep -o '[0-9]{1,}\.[0-9]{1,}')
#PG_VERSION=9.5
PG_VERSION=$(psql --version | egrep -o '[0-9]{1,}\.[0-9]{1,}')

if ! grep --quiet "psql-" ~/.bash_profile; then
    echo "Adding PostGres path vars to profile..."
    echo "export PATH=\$PATH:/usr/pgsql-$PG_VERSION/bin" >> ~/.bash_profile
    source ~/.bash_profile
fi

# Get the configuration for the Database
source $HOOT_HOME/conf/database/DatabaseConfig.sh

# Make sure that we are working on localhost before trying to configure and start postgres
if [ "$DB_HOST" = "localhost" ] || [ "$DB_HOST_OSMAPI" = "localhost" ]; then

  echo "## Working on " $DB_HOST

  # Test to see if postgres cluster already created
  if ! sudo -u postgres test -f /var/lib/pgsql/$PG_VERSION/data/PG_VERSION; then
    sudo PGSETUP_INITDB_OPTIONS="-E 'UTF-8' --lc-collate='en_US.UTF-8' --lc-ctype='en_US.UTF-8'" /usr/pgsql-$PG_VERSION/bin/postgresql95-setup initdb
  fi

  sudo systemctl start postgresql-$PG_VERSION
  sudo systemctl enable postgresql-$PG_VERSION

  # configure Postgres settings
  PG_HB_CONF=/var/lib/pgsql/$PG_VERSION/data/pg_hba.conf
  if ! sudo -u postgres grep -i --quiet hoot $PG_HB_CONF; then
      sudo -u postgres cp $PG_HB_CONF $PG_HB_CONF.orig
      sudo -u postgres sed -i '1ihost    all            hoot            127.0.0.1/32            md5' $PG_HB_CONF
      sudo -u postgres sed -i '1ihost    all            hoot            ::1/128                 md5' $PG_HB_CONF
  fi

  POSTGRES_CONF=/var/lib/pgsql/$PG_VERSION/data/postgresql.conf
  if ! sudo -u postgres grep -i --quiet HOOT $POSTGRES_CONF; then
      sudo -u postgres cp $POSTGRES_CONF $POSTGRES_CONF.orig
      sudo -u postgres sed -i s/^max_connections/\#max_connections/ $POSTGRES_CONF
      sudo -u postgres sed -i s/^shared_buffers/\#shared_buffers/ $POSTGRES_CONF
      sudo -u postgres bash -c "cat >> $POSTGRES_CONF" <<EOT
#--------------
# Hoot Settings
#--------------
max_connections = 1000
shared_buffers = 1024MB
max_files_per_process = 1000
work_mem = 16MB
maintenance_work_mem = 256MB
#checkpoint_segments = 20
autovacuum = off
EOT
  fi

  echo "Restarting postgres"
  sudo systemctl restart postgresql-$PG_VERSION

fi # End localhost check


