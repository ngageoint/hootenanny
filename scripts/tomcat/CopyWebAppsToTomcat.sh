#!/bin/bash

set -e

echo Deploying web application files...

# Set this for use later
export OS_NAME="$(lsb_release -i -s)"

if [ "$OS_NAME" == "Ubuntu" ]; then
  source ~/.profile
else # Centos
  source ~/.bash_profile
fi

echo "user: "$USER
sudo -u $USER -g tomcat rm -f $TOMCAT8_HOME/webapps/hoot-services.war

# Need to make sure we don't die due to nfs funkyness
sudo -u $USER -g tomcat rm -rf $TOMCAT8_HOME/webapps/hoot-services/
sudo -u $USER -g tomcat rm -rf $TOMCAT8_HOME/webapps/hoot-services/

sudo -u $USER -g tomcat cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT8_HOME/webapps/hoot-services.war

# Copy hootenanny-ui web files
sudo -u $USER -g tomcat rm -rf $TOMCAT8_HOME/webapps/hootenanny-id-legacy/
sudo -u $USER -g tomcat cp -R hoot-ui/dist $TOMCAT8_HOME/webapps/hootenanny-id-legacy
sudo -u $USER -g tomcat mkdir -p $TOMCAT8_HOME/webapps/hootenanny-id-legacy/data
sudo -u $USER -g tomcat cp hoot-ui/data/osm-plus-taginfo.csv $TOMCAT8_HOME/webapps/hootenanny-id-legacy/data
sudo -u $USER -g tomcat cp hoot-ui/data/tdsv61_field_values.json $TOMCAT8_HOME/webapps/hootenanny-id-legacy/data

#Copy hootenanny-ui2x we files
sudo -u $USER -g tomcat rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
sudo -u $USER -g tomcat cp -R hoot-ui-2x/dist $TOMCAT8_HOME/webapps/hootenanny-id

echo Web application files deployed.
