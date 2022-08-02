#!/usr/bin/env bash

set -e

# Configure OAuth2 redirect for port 8888
sudo -u vagrant sed -i "\|https://localhost:8080/login.html|s|^|# |" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo -u vagrant sed -i "\|https://localhost:8443/hootenanny-id/login.html|s|^# ||" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo systemctl restart tomcat8
