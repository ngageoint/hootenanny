#!/bin/bash

set -e

echo Deploying web application files...

# Set this for use later
export OS_NAME=$(awk -F= '/^NAME/{print $2}' /etc/os-release)

if [ "$OS_NAME" == 'Ubuntu' ]; then
  source ~/.profile
else # Centos
  source ~/.bash_profile
fi

rm -f $TOMCAT8_HOME/webapps/hoot-services.war

# Need to make sure we don't die due to nfs funkyness
rm -rf $TOMCAT8_HOME/webapps/hoot-services/
rm -rf $TOMCAT8_HOME/webapps/hoot-services/

cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT8_HOME/webapps/hoot-services.war

rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
cp -R hoot-ui/dist $TOMCAT8_HOME/webapps/hootenanny-id
mkdir -p $TOMCAT8_HOME/webapps/hootenanny-id/data
cp hoot-ui/data/osm-plus-taginfo.csv $TOMCAT8_HOME/webapps/hootenanny-id/data
cp hoot-ui/data/tdsv61_field_values.json $TOMCAT8_HOME/webapps/hootenanny-id/data

echo Web application files deployed.
