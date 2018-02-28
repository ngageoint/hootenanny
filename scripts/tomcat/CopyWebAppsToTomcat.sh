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

TOMCAT_USER=tomcat8
VMUSER=`id -u -n`
if [ "$VMUSER" = 'vagrant' ]
then
  TOMCAT_USER=$VMUSER
fi

sudo -u $TOMCAT_USER rm -f $TOMCAT8_HOME/webapps/hoot-services.war

# Need to make sure we don't die due to nfs funkyness
sudo -u $TOMCAT_USER rm -rf $TOMCAT8_HOME/webapps/hoot-services/
sudo -u $TOMCAT_USER rm -rf $TOMCAT8_HOME/webapps/hoot-services/

sudo -u $TOMCAT_USER cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT8_HOME/webapps/hoot-services.war

sudo -u $TOMCAT_USER rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
sudo -u $TOMCAT_USER cp -R hoot-ui/dist $TOMCAT8_HOME/webapps/hootenanny-id
sudo -u $TOMCAT_USER mkdir -p $TOMCAT8_HOME/webapps/hootenanny-id/data
sudo -u $TOMCAT_USER cp hoot-ui/data/osm-plus-taginfo.csv $TOMCAT8_HOME/webapps/hootenanny-id/data
sudo -u $TOMCAT_USER cp hoot-ui/data/tdsv61_field_values.json $TOMCAT8_HOME/webapps/hootenanny-id/data

echo Web application files deployed.
