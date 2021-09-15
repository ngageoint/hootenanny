#!/usr/bin/env bash

set -e

# Configure OAuth redirect for port 8888
sudo -u tomcat sed -i "s|oauthRedirectURL=http://localhost:8080/login.html|# oauthRedirectURL=http://localhost:8080/login.html|" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/conf/hoot-services.conf
sudo -u tomcat sed -i "s|# oauthRedirectURL=http://localhost:8888/hootenanny-id/login.html|oauthRedirectURL=http://localhost:8888/hootenanny-id/login.html|" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/conf/hoot-services.conf
sudo systemctl restart tomcat8
