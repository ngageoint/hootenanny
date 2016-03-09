#!/bin/bash

set -e

BASEDIR=$(dirname $0)
if [ -f $BASEDIR/../Config.sh ]; then
  source $BASEDIR/../Config.sh
else
  # must be running in the hoot dir
  export HOOT_HOME=$(dirname $0)
  source $HOOT_HOME/conf/Config.sh
fi

function printOnError {
    OUT=/tmp/HootDeploy$RANDOM.log
    eval $* &> $OUT || (cat $OUT; rm -f $OUT; false)
    rm -f $OUT
}

echo Tomcat home: $TOMCAT6_HOME

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)

$HOOT_HOME/scripts/StopTomcat.sh

rm -rf $TOMCAT6_HOME/webapps/hoot-services/
cp $HOOT_HOME/hoot-services/target/hoot-services-*.war $TOMCAT6_HOME/webapps/hoot-services.war
unzip -q -d $TOMCAT6_HOME/webapps/hoot-services $TOMCAT6_HOME/webapps/hoot-services.war
cp $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf $TOMCAT6_HOME/webapps/hoot-services/WEB-INF/classes/conf

rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/
cp -R $HOOT_HOME/hoot-ui $TOMCAT6_HOME/webapps/hootenanny-id

# set the P2P port as described in Config.sh
sed -i s/8096/$P2P_PORT/g $TOMCAT6_HOME/webapps/hootenanny-id/data/hootConfig.json

# set the node-mapnik-server port as described in Config.sh
sed -i s/8000/$NODE_MAPNIK_SERVER_PORT/g $TOMCAT6_HOME/webapps/hootenanny-id/data/hootConfig.json

rm webapps/hootenanny-id/css/img
cp -R $TOMCAT6_HOME/webapps/hootenanny-id/dist/img $TOMCAT6_HOME/webapps/hootenanny-id/css/

$HOOT_HOME/scripts/StartTomcat.sh

echo
echo "= Restarting node-mapnik-server ="
echo
$HOOT_HOME/scripts/InstallRunNodeMapnikServer.sh

