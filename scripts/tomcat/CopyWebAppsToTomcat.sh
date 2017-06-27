#!/bin/bash

set -e

echo Deploying web application files...

source ~/.profile

rm -f $TOMCAT8_HOME/webapps/hoot-services.war

# Need to make sure we don't die due to nfs funkyness
rm -rf $TOMCAT8_HOME/webapps/hoot-services/
rm -rf $TOMCAT8_HOME/webapps/hoot-services/

cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT8_HOME/webapps/hoot-services.war

cd hoot-ui
make clean && make
rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
cp -R dist $TOMCAT8_HOME/webapps/hootenanny-id
mkdir -p $TOMCAT8_HOME/webapps/hootenanny-id/data
cp hoot-ui/data/osm-plus-taginfo.csv $TOMCAT8_HOME/webapps/hootenanny-id/data
cp hoot-ui/data/tdsv61_field_values.json $TOMCAT8_HOME/webapps/hootenanny-id/data

echo Web application files deployed.
