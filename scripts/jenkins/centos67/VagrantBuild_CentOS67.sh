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

export LIBRARY_PATH=$LIBRARY_PATH:/usr/pgsql-9.2/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/pgsql-9.2/lib

aclocal && autoconf && autoheader && automake --add-missing && ./configure --quiet --with-rnd --with-services

echo "Building Hoot... "
echo "Will take several extra minutes to build the training data the initial time Hootenanny is installed only."
make -s clean && make -sj$(nproc)

# Make sure that these directories exist. This is done by VagrantProvision.sh but will be wiped out if the user
mkdir -p $HOOT_HOME/ingest/processed
mkdir -p $HOOT_HOME/upload

#make -sj$(nproc) docs &> /dev/null || true
#make -sj$(nproc) docs

hoot version
