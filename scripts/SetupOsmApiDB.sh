#!/bin/bash
set -e

# create the libpgosm.so lib on the fly
#echo "Creating libpgosm lib"
#pushd . >& /tmp/osmapidb.log
#cd $HOOT_HOME/scripts/osmapidb
#cc -I `pg_config --includedir` -I `pg_config --includedir-server` -fPIC -O3 -DUSE_PGSQL -c -o quadtile-pgsql.o quadtile.c
#cc -I `pg_config --includedir` -I `pg_config --includedir-server` -fPIC -O3 -DUSE_PGSQL -c -o maptile-pgsql.o maptile.c
#cc -I `pg_config --includedir` -I `pg_config --includedir-server` -fPIC -O3 -DUSE_PGSQL -c -o xid_to_int4-pgsql.o xid_to_int4.c
#cc -shared -o libpgosm.so quadtile-pgsql.o maptile-pgsql.o xid_to_int4-pgsql.o

# put libpgosm.so in tmp (hope that is a safe place)
#echo "Installing libpgosm lib"
#cp -f libpgosm.so /tmp
#rm -rf maptile-pgsql.o quadtile-pgsql.o xid_to_int4-pgsql.o libpgosm.so
#popd >> /tmp/osmapidb.log

source $HOOT_HOME/conf/DatabaseConfig.sh

# setup db, user, and password to avoid password prompt
export DB_NAME=$PGDATABASE
export AUTH="-h $PGHOST -p $PGPORT -U $PGUSER"

# see if old db osmapi_test exists
export flag=`psql $AUTH -lqt | cut -d \| -f 1 | grep -w $PGDATABASE | wc -l`
if [ "$flag" = "1" ]; then
#  echo "Dropping old osm api db"
  dropdb $AUTH $PGDATABASE
fi

# create the osm apu db from the blank osm api db script
#echo "Creating osm api db"
createdb $AUTH $PGDATABASE
psql $AUTH -d $PGDATABASE -f $HOOT_HOME/scripts/osmapidb/blank_osmapidb.sql $PGDATABASE >& /tmp/osmapidb.log
