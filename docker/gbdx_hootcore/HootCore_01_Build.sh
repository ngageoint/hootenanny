#!/usr/bin/env bash

set -e

#####
# NOTE: This is a VERY cut down version of Hootenanny for a single purpose
#
# It runs on Centos7
#
# It DOES NOT include a database or a UI
#
#
# MattJ


###################################################
# VERY IMPORTANT: Set the $HOOT_HOME environment  #
# variable prior to running this script if ~/hoot #
# isn't the correct location for HOOT_HOME        #
###################################################
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi
echo HOOT_HOME: $HOOT_HOME
#################################################


#####
# Temp change until we get the C++11 support into develop
cd $HOOT_HOME
cp LocalConfig.pri.orig LocalConfig.pri
echo "QMAKE_CXXFLAGS += -std=c++11" >> LocalConfig.pri
#####

# Not sure if we need this:
# if ! mocha --version &>/dev/null; then
#     echo "### Installing mocha for translations test..."
#     npm install --silent -g mocha@3.5.3
#     # Clean up after the npm install
#     rm -rf ~/tmp
# fi

echo "SetupEnv.sh"
source ./SetupEnv.sh

echo "Building Hoot"
# Going to remove this so that it gets updated
if [ -f missing ]; then
  rm -f missing
fi

# We could just hardcode this to 9.5 but it is removed for the moment
# PG_VERSION=$(psql --version | egrep -o '[0-9]{1,}\.[0-9]{1,}')
#   --with-postgresql=/usr/pgsql-$PG_VERSION/bin/pg_config \

aclocal && autoconf && autoheader && automake --add-missing --copy

./configure \
  --with-rnd \
  --without-services \
  --without-postgresql \
  --prefix=/usr/ \
  --datarootdir=/usr/share/hootenanny/ \
  --docdir=/usr/share/doc/hootenanny/ \
  --localstatedir=/var/lib/hootenanny/ \
  --libdir=/usr/lib64 \
  --sysconfdir=/etc/


echo "Building Hoot... "
make -s clean && make -sj$(nproc)

# This stops the install dieing if it cant copy PDF files.
touch docs/empty.pdf

##########################################
