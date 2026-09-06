#!/bin/bash
set -euo pipefail

export PATH="/usr/local/bin:/usr/local/sbin:/usr/pgsql-$PG_MAJOR_VERSION/bin:$PATH"
export GDAL_CONFIG="${GDAL_CONFIG:-/usr/local/bin/gdal-config}"

if [ -f "/opt/rh/devtoolset-$DEVTOOLSET_VERSION/enable" ]; then
    . /opt/rh/devtoolset-$DEVTOOLSET_VERSION/enable
fi


source ./SetupEnv.sh
source conf/database/DatabaseConfig.sh

export PATH="/usr/local/bin:/usr/local/sbin:/usr/pgsql-$PG_MAJOR_VERSION/bin:$PATH"
export LD_LIBRARY_PATH="/usr/pgsql-$PG_MAJOR_VERSION/lib:$LD_LIBRARY_PATH"

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
    for build_dir in tbs tgs hoot-core hoot-core-test hoot-cmd hoot-js; do
        if [ -d "$build_dir" ]; then
            find "$build_dir" -path '*/tmp/*' -name '*.o' -size 0 -delete
        fi
    done
    make core -j$(nproc)
    copy_war_to_tomcat
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

export GDAL_DATA=$("$GDAL_CONFIG" --datadir)

rm -f core-services-building.txt
npm start --prefix "${HOOT_HOME}/node-export-server" &
start_tomcat
