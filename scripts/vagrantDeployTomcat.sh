#!/bin/bash

export TOMCAT6_HOME=/var/lib/tomcat6

rm $TOMCAT6_HOME/webapps/hoot-services.war
cp hoot-services/target/hoot-services-*.war $TOMCAT6_HOME/webapps/hoot-services.war
rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/
cp -R hoot-ui $TOMCAT6_HOME/webapps/hootenanny-id
rm -rf $TOMCAT6_HOME/webapps/hootenanny-id/css/img
cp -R $TOMCAT6_HOME/webapps/hootenanny-id/dist/img $TOMCAT6_HOME/webapps/hootenanny-id/css/
