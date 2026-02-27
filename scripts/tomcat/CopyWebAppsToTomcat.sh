#!/bin/bash

set -e

echo Deploying web application files...

# Set this for use later
export OS_NAME="$(lsb_release -i -s)"

if [ "$OS_NAME" == "Ubuntu" ]; then
  source ~/.profile
else # Centos/Rocky/AlmaLinux
  source ~/.bash_profile
fi

# Support both Tomcat 8 (EL7) and Tomcat 9 (EL9)
if [ -n "${TOMCAT9_HOME:-}" ]; then
  TOMCAT_HOME=$TOMCAT9_HOME
elif [ -n "${TOMCAT8_HOME:-}" ]; then
  TOMCAT_HOME=$TOMCAT8_HOME
else
  echo "Error: Neither TOMCAT9_HOME nor TOMCAT8_HOME is set"
  exit 1
fi

sudo rm -f $TOMCAT_HOME/webapps/hoot-services.war

# Need to make sure we don't die due to nfs funkyness
sudo rm -rf $TOMCAT_HOME/webapps/hoot-services/
sudo rm -rf $TOMCAT_HOME/webapps/hoot-services/

sudo cp `ls -t hoot-services/target/hoot-services-*.war | sed -n 1p` $TOMCAT_HOME/webapps/hoot-services.war
sudo chown $USER:tomcat $TOMCAT_HOME/webapps/hoot-services.war

#Copy hootenanny-ui2x we files
sudo rm -rf $TOMCAT_HOME/webapps/hootenanny-id/
sudo cp -R hoot-ui-2x/dist $TOMCAT_HOME/webapps/hootenanny-id/
sudo chown -R $USER:tomcat $TOMCAT_HOME/webapps/hootenanny-id/

echo Web application files deployed.
