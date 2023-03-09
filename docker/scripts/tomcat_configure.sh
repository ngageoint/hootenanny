#!/bin/bash
set -euo pipefail

if ! grep -i --quiet 'ingest/processed' $TOMCAT_SERVER; then
    sed -i 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/userfiles\/ingest\/processed\" path=\"\/static\" \/>\n      &@' $TOMCAT_SERVER
fi

# OSM OAuth2 requires redirect URLs to use https
# Generate a keystore with a self-signed cert for localhost
if [ ! -f $TOMCAT_CONFIG/localhost-rsa.jks ]; then
    keytool -genkey -keyalg RSA -noprompt -alias tomcat -dname "CN=localhost, OU=NA, O=NA, L=NA, S=NA, C=NA" -keystore $TOMCAT_CONFIG/localhost-rsa.jks -validity 9999 -storepass changeme -keypass changeme
    chown root:tomcat $TOMCAT_CONFIG/localhost-rsa.jks
fi

# Enable SSL using self-signed cert
if ! grep -i --quiet 'certificateKeyAlias="tomcat"' ${TOMCAT_CONFIG}/server.xml; then
    sed -i.bak 's@<!-- Define an AJP 1.3 Connector on port 8009 -->@<Connector port="8443" protocol="org.apache.coyote.http11.Http11NioProtocol" maxThreads="150" SSLEnabled="true">\n        <SSLHostConfig>\n            <Certificate certificateKeyAlias="tomcat" certificateKeystorePassword="changeme" certificateKeystoreFile="/etc/tomcat8/localhost-rsa.jks" type="RSA" />\n        </SSLHostConfig>\n    </Connector>\n\n    &@' ${TOMCAT_CONFIG}/server.xml
fi

mkdir -p $TOMCAT8_HOME/webapps/{hootenanny-id,hoot-services}
chown -R $HOOT_USER:tomcat $TOMCAT8_HOME/webapps/hootenanny-id/
chown -R $HOOT_USER:tomcat $TOMCAT8_HOME/webapps/hoot-services/

