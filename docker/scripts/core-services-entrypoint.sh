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

copy_war_and_frontend_to_tomcat()
{
    rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
    cp -R hoot-ui-2x/dist $TOMCAT8_HOME/webapps/hootenanny-id/
    chown -R $HOOT_USER:tomcat $TOMCAT8_HOME/webapps/hootenanny-id/

    rm -f $TOMCAT8_HOME/webapps/hoot-services.war
    rm -rf $TOMCAT8_HOME/webapps/hoot-services/
    cp hoot-services/target/hoot-services-$HOOT_USER.war $TOMCAT8_HOME/webapps/hoot-services.war
}

if [ ! -f configure ]; then
    ./docker/scripts/core-services-configure.sh
fi

if [ "${HOOT_BUILD_CORE:-0}" = "1" ] || [ ! -f ./bin/hoot.bin ]; then
    # everything the core target does except build servces
    make -f Makefile.hoot -j$(nproc) \
        makedirs js-make qt-make model-build \
        conf/dictionary/words.sqlite \
        conf/dictionary/WordsAbridged.sqlite \
        hoot-services/src/main/resources/language-translation/langdetect-183.bin
fi

if [ "${HOOT_BUILD_HOOT_SERVICES_UI:-0}" = "1" ] || [ ! -f hoot-services/target/hoot-services-$HOOT_USER.war ]; then
    make services-build
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

copy_war_and_frontend_to_tomcat
npm start --prefix "${HOOT_HOME}/node-export-server" &
start_tomcat
