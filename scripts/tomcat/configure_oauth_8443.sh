#!/usr/bin/env bash

set -e

# Auto-detect Tomcat version (support both Tomcat 8 and Tomcat 9)
if [ -d /var/lib/tomcat9 ]; then
    TOMCAT_HOME=/var/lib/tomcat9
    TOMCAT_SERVICE=tomcat9
elif [ -d /var/lib/tomcat8 ]; then
    TOMCAT_HOME=/var/lib/tomcat8
    TOMCAT_SERVICE=tomcat8
else
    echo "Error: Neither Tomcat 8 nor Tomcat 9 installation found"
    exit 1
fi

echo "Configuring OAuth2 redirect for $TOMCAT_SERVICE on port 8443..."

# Configure OAuth2 redirect for port 8443
sudo sed -i "\|https://localhost:8080/login.html|s|^|# |" $TOMCAT_HOME/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo sed -i "\|https://localhost:8443/hootenanny-id/login.html|s|^# ||" $TOMCAT_HOME/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo systemctl restart $TOMCAT_SERVICE
