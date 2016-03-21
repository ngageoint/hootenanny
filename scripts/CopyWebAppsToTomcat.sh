#!/bin/bash

set -e

BASEDIR=$(dirname $0)
if [ -f $BASEDIR/../Config.sh ]; then
  # We're running tests in the CI environment.
  source $BASEDIR/../Config.sh
else
  # We're running tests in a development environment.
  source $HOOT_HOME/conf/ServerConfig.sh
fi

echo Deploying web application files...

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

rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/css/img
cp -R $TOMCAT6_HOME/webapps/hootenanny-id/dist/img $TOMCAT6_HOME/webapps/hootenanny-id/css/

echo Web application files deployed.
