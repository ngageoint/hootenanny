#!/bin/bash

set -e

echo Deploying web application files...

source ~/.profile

rm -f $TOMCAT8_HOME/webapps/hoot-services.war

# Need to make sure we don't die due to nfs funkyness
rm -rf $TOMCAT8_HOME/webapps/hoot-services/
rm -rf $TOMCAT8_HOME/webapps/hoot-services/

cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT8_HOME/webapps/hoot-services.war

rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
cp -R hoot-ui $TOMCAT8_HOME/webapps/hootenanny-id

rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/css/img
cp -R $TOMCAT8_HOME/webapps/hootenanny-id/dist/img $TOMCAT8_HOME/webapps/hootenanny-id/css/

#rm -f $TOMCAT8_HOME/webapps/hoot-services.war
#rm -rf $TOMCAT8_HOME/webapps/hoot-services/
#cp $HOOT_HOME/hoot-services/target/hoot-services-*.war $TOMCAT8_HOME/webapps/hoot-services.war
#unzip -q -d $TOMCAT8_HOME/webapps/hoot-services $TOMCAT8_HOME/webapps/hoot-services.war
#cp $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf $TOMCAT8_HOME/webapps/hoot-services/WEB-INF/classes/conf
#rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
#cp -R $HOOT_HOME/hoot-ui $TOMCAT8_HOME/webapps/hootenanny-id
#rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/css/img
#cp -R $TOMCAT8_HOME/webapps/hootenanny-id/dist/img $TOMCAT8_HOME/webapps/hootenanny-id/css/

echo Web application files deployed.
