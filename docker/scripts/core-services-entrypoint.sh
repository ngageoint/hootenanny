#!/bin/bash
set -euo pipefail

. /opt/rh/devtoolset-$DEVTOOLSET_VERSION/enable


source ./SetupEnv.sh
source conf/database/DatabaseConfig.sh

start_tomcat()
{
    . /etc/tomcat8/tomcat8.conf
    . /etc/sysconfig/tomcat8
    NAME= /usr/libexec/tomcat8/server start
}

stop_tomcat()
{
    /usr/libexec/tomcat8/server stop
}

copy_war_to_tomcat()
{
    rm -rf $TOMCAT8_HOME/webapps/hoot-services/*
    cp -R hoot-services/target/hoot-services-$HOOT_USER/* $TOMCAT8_HOME/webapps/hoot-services
}

touch core-services-building.txt

if [ "${HOOT_CLEAN:-0}" = "1" ]; then
    ./docker/scripts/core-services-configure.sh
    make clean
fi;

if [ "${HOOT_BUILD_CORE:-0}" = "1" ] || [ ! -f ./bin/hoot.bin ]; then
    ./docker/scripts/core-services-configure.sh
    make core -j$(nproc)
fi

if [ "${HOOT_BUILD_HOOT_SERVICES:-0}" = "1" ] || [ ! -f hoot-services/target/hoot-services-$HOOT_USER.war ]; then
    make services-build
    copy_war_to_tomcat
fi;

# translation server is tied to services so need to build schema files and its dependencies
if [ "${HOOT_BUILD_JS_SCHEMA:-0}" = "1" ] || [ ! -f translations/tds71_schema.js ]; then
    make -f Makefile.hoot js-make
fi

if [ "${HOOT_BUILD_TRANSLATION_SERVER:-0}" = "1" ] || [ ! -d translations/node_modules ]; then
    make -f Makefile.hoot translations-test
fi;

if [ "${HOOT_BUILD_NODE_EXPORT_SERVER:-0}" = "1" ] || [ ! -d node-export-server/node_modules ]; then
    pushd node-export-server;
    npm install;
    popd
fi;

export GDAL_DATA=$(gdal-config --datadir)

rm -f core-services-building.txt
npm start --prefix "${HOOT_HOME}/node-export-server" &
start_tomcat
