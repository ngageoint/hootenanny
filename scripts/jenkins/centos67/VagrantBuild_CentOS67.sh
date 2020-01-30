#!/usr/bin/env bash
set -e

source ~/.bashrc
cd hoot
source ./SetupEnv.sh

echo "### Configuring Hoot..."
echo HOOT_HOME: $HOOT_HOME

# Going to remove this so that it gets updated
if [ -f missing ]; then
  rm -f missing
fi

# Remove C++11 since the compiler on Centos67 doesn't support it.
sed -i s/"-std=c++11"// Configure.pri.in

# Sort out what version of Postgres we have - MJ
PG_VERSION=`ls /etc/init.d | grep postgresql- | sort | tail -1 | egrep -o '[0-9]{1,}\.[0-9]{1,}'`

aclocal && autoconf && autoheader && automake --add-missing && \
  ./configure --quiet --with-rnd --with-services --with-josm --with-postgresql=/usr/pgsql-$PG_VERSION/bin/pg_config

echo "Building Hoot... "
echo "Will take several extra minutes to build the training data the initial time Hootenanny is installed only."
make -s clean && make -sj$(nproc)

# Make sure that these directories exist. This is done by VagrantProvision.sh but will be wiped out if the user
mkdir -p $HOOT_HOME/ingest/processed
mkdir -p $HOOT_HOME/upload

#make -sj$(nproc) docs &> /dev/null || true
make -sj$(nproc) docs

# Now undo the C++11 removal. This keeps Git happy the next time we try to update.
git checkout -- Configure.pri.in

hoot version
