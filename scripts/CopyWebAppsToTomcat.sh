#!/bin/bash

set -e

echo Deploying web application files...

source ~/.profile

rm -f $TOMCAT6_HOME/webapps/hoot-services.war &> /dev/null || true

# Need to make sure we don't die due to nfs funkyness
rm -rf $TOMCAT6_HOME/webapps/hoot-services/ &> /dev/null || true
rm -rf $TOMCAT6_HOME/webapps/hoot-services/ &> /dev/null || true

cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT6_HOME/webapps/hoot-services.war

rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/ &> /dev/null || true
cp -R hoot-ui $TOMCAT6_HOME/webapps/hootenanny-id

rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/css/img &> /dev/null || true
cp -R $TOMCAT6_HOME/webapps/hootenanny-id/dist/img $TOMCAT6_HOME/webapps/hootenanny-id/css/

#rm -f $TOMCAT6_HOME/webapps/hoot-services.war
#rm -rf $TOMCAT6_HOME/webapps/hoot-services/
#cp $HOOT_HOME/hoot-services/target/hoot-services-*.war $TOMCAT6_HOME/webapps/hoot-services.war
#unzip -q -d $TOMCAT6_HOME/webapps/hoot-services $TOMCAT6_HOME/webapps/hoot-services.war
#cp $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf $TOMCAT6_HOME/webapps/hoot-services/WEB-INF/classes/conf
#rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/
#cp -R $HOOT_HOME/hoot-ui $TOMCAT6_HOME/webapps/hootenanny-id
#rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/css/img
#cp -R $TOMCAT6_HOME/webapps/hootenanny-id/dist/img $TOMCAT6_HOME/webapps/hootenanny-id/css/

echo Web application files deployed.
