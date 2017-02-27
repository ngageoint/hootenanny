#!/bin/bash

export TOMCAT8_HOME=/usr/share/tomcat8

rm -rf $TOMCAT8_HOME/webapps/hoot-services
rm -f $TOMCAT8_HOME/webapps/hoot-services.war
cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT8_HOME/webapps/hoot-services.war
rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
cp -R hoot-ui $TOMCAT8_HOME/webapps/hootenanny-id
rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/css/img
cp -R $TOMCAT8_HOME/webapps/hootenanny-id/dist/img $TOMCAT8_HOME/webapps/hootenanny-id/css/
