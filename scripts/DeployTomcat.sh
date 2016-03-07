#!/bin/bash

set -e

BASEDIR=$(dirname $0)
source $BASEDIR/../Config.sh

function printOnError {
    OUT=/tmp/HootDeploy$RANDOM.log
    eval $* &> $OUT || (cat $OUT; rm -f $OUT; false)
    rm -f $OUT
}

echo Tomcat home: $TOMCAT6_HOME

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)

$NIGHTLY_HOME/scripts/StopTomcat.sh

cd $TOMCAT6_HOME
rm -rf webapps/hoot-services/
cp $HOOT_HOME/hoot-services/target/hoot-services-*.war webapps/hoot-services.war
unzip -q -d webapps/hoot-services webapps/hoot-services.war
cp $NIGHTLY_HOME/local.conf webapps/hoot-services/WEB-INF/classes/conf

rm -rf webapps/hootenanny-id/
cp -R $HOOT_HOME/hoot-ui webapps/hootenanny-id

# Activate the EDG iD plugin
$NIGHTLY_HOME/scripts/ActivateEGDplugin.sh

# set the P2P port as described in Config.sh
sed -i s/8096/$P2P_PORT/g webapps/hootenanny-id/data/hootConfig.json

# set the node-mapnik-server port as described in Config.sh
sed -i s/8000/$NODE_MAPNIK_SERVER_PORT/g webapps/hootenanny-id/data/hootConfig.json

rm webapps/hootenanny-id/css/img
cp -R webapps/hootenanny-id/dist/img webapps/hootenanny-id/css/

$NIGHTLY_HOME/scripts/StartTomcat.sh

echo
echo "= Restarting node-mapnik-server ="
echo
$NIGHTLY_HOME/scripts/InstallRunNodeMapnikServer.sh

