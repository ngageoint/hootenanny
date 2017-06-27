#!/bin/bash

export TOMCAT8_HOME=/usr/share/tomcat8

cd hoot-ui
make clean && make
rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
cp -R dist $TOMCAT8_HOME/webapps/hootenanny-id
mkdir -p $TOMCAT8_HOME/webapps/hootenanny-id/data
cp data/osm-plus-taginfo.csv $TOMCAT8_HOME/webapps/hootenanny-id/data
cp data/tdsv61_field_values.json $TOMCAT8_HOME/webapps/hootenanny-id/data
