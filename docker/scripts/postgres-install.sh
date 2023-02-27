#!/bin/bash
set -eu

POSTGRES_VERSION="${1}"

yum -q -y install \
    postgresql${POSTGRES_VERSION} \
    postgresql${POSTGRES_VERSION}-contrib \
    postgresql${POSTGRES_VERSION}-server

alternatives --install /usr/bin/createdb pgsql-createdb "/usr/pgsql-${POSTGRES_VERSION}/bin/createdb" 500
alternatives --install /usr/bin/createuser pgsql-createuser "/usr/pgsql-${POSTGRES_VERSION}/bin/createuser" 500
alternatives --install /usr/bin/initdb pgsql-initdb "/usr/pgsql-${POSTGRES_VERSION}/bin/initdb" 500
alternatives --install /usr/bin/pg_config pgsql-pg_config "/usr/pgsql-${POSTGRES_VERSION}/bin/pg_config" 500
alternatives --install /usr/bin/pg_ctl pgsql-pg_ctl "/usr/pgsql-${POSTGRES_VERSION}/bin/pg_ctl" 500
alternatives --install /usr/bin/pg_isready pgsql-pg_isready "/usr/pgsql-${POSTGRES_VERSION}/bin/pg_isready" 500
alternatives --install /usr/bin/postgres pgsql-postgres "/usr/pgsql-${POSTGRES_VERSION}/bin/postgres" 500

# init and start postgres
if [ ! -e /var/lib/pgsql/$POSTGRES_VERSION/data/PG_VERSION ]; then
    PGSETUP_INITDB_OPTIONS="-E UTF-8 --lc-collate=en_US.UTF-8 --lc-ctype=en_US.UTF-8" \
    su postgres -c "initdb -D $PGDATA --locale $LANG"
fi

su postgres -c "pg_ctl -D $PGDATA -s -w start"


while ! /usr/pgsql-$POSTGRES_VERSION/bin/pg_isready; do
    echo "Waiting for postgres to start"
    sleep 1
done

# create Hoot services db
if ! psql -lqt | cut -d \| -f 1 | grep -qw hoot; then
    su postgres -c "createuser --superuser hoot || true";
    su postgres -c "psql -c \"ALTER USER hoot WITH PASSWORD 'hoottest';\""
    su postgres -c "createdb hoot --owner=hoot"
    su postgres -c "psql -d hoot -c \"CREATE EXTENSION hstore;\""
fi

su postgres -c "pg_ctl -D $PGDATA -s -m fast -w stop"

# configure Postgres settings
PG_HB_CONF=/var/lib/pgsql/$POSTGRES_VERSION/data/pg_hba.conf
if ! grep -i --quiet hoot $PG_HB_CONF; then
    sed -i '1ihost    all            all            0.0.0.0/0            md5' $PG_HB_CONF
fi
POSTGRES_CONF=/var/lib/pgsql/$POSTGRES_VERSION/data/postgresql.conf
if ! grep -i --quiet HOOT $POSTGRES_CONF; then
    sed -i "s/^max_connections/\#max_connections/" $POSTGRES_CONF
    sed -i "s/^shared_buffers/\#shared_buffers/" $POSTGRES_CONF
    cat >> $POSTGRES_CONF <<EOT

#--------------
# Hoot Settings
#--------------
listen_addresses = '0.0.0.0'
max_connections = 1000
shared_buffers = 1024MB
max_files_per_process = 1000
work_mem = 16MB
maintenance_work_mem = 256MB
autovacuum = off
EOT
fi
