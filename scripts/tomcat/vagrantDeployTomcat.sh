#!/bin/bash

export TOMCAT8_HOME=/usr/share/tomcat8

rm -rf $TOMCAT8_HOME/webapps/hoot-services
rm -f $TOMCAT8_HOME/webapps/hoot-services.war
cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT8_HOME/webapps/hoot-services.war
rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
cp -R hoot-ui/dist $TOMCAT8_HOME/webapps/hootenanny-id
mkdir -p $TOMCAT8_HOME/webapps/hootenanny-id/data
cp hoot-ui/data/osm-plus-taginfo.csv $TOMCAT8_HOME/webapps/hootenanny-id/data
cp hoot-ui/data/tdsv61_field_values.json $TOMCAT8_HOME/webapps/hootenanny-id/data
