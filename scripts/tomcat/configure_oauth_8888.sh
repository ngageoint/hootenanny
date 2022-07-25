#!/usr/bin/env bash

set -e

# Configure OAuth2 redirect for port 8888
sudo -u vagrant sed -i "\|Kylc2jcDm7T7gI6YGvcQNq819bpd1GM0ymFfhxk1JAs|s|^|# |" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo -u vagrant sed -i "\|5gmvfsMG5EFyG62KWD8ZSLXxGdWEsXtrEuHwz_i6IoM|s|^|# |" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo -u vagrant sed -i "\|https://localhost:8080/login.html|s|^|# |" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties

sudo -u vagrant sed -i "\|eRchZ9Oy0xzvb8UTxJlv0Mp99KL1rsfob39Xu5qylfc|s|^# ||" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo -u vagrant sed -i "\|NhbtYUSHjnCfa6XEwkcu_ILp6E6mG6zw4nR-dx_gg10|s|^# ||" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo -u vagrant sed -i "\|https://localhost:8443/hootenanny-id/login.html|s|^# ||" /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/oauth2-client.properties
sudo systemctl restart tomcat8
