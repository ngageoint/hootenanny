#!/usr/bin/env bash

set -e

TOMCAT_CONFIG=/etc/tomcat8

# OSM OAuth2 requires redirect URLs to use https
# Generate a keystore with a self-signed cert for localhost
if [ ! -f $TOMCAT_CONFIG/localhost-rsa.jks ]; then
sudo keytool -genkey -keyalg RSA -noprompt -alias tomcat -dname "CN=localhost, OU=NA, O=NA, L=NA, S=NA, C=NA" -keystore $TOMCAT_CONFIG/localhost-rsa.jks -validity 9999 -storepass changeme -keypass changeme
sudo chown root:tomcat $TOMCAT_CONFIG/localhost-rsa.jks
fi

# Enable SSL using self-signed cert
if ! grep -i --quiet 'certificateKeyAlias="tomcat"' ${TOMCAT_CONFIG}/server.xml; then
echo "Enabling SSL for Tomcat..."
sudo sed -i.bak 's@<!-- Define an AJP 1.3 Connector on port 8009 -->@<Connector port="8443" protocol="org.apache.coyote.http11.Http11NioProtocol" maxThreads="150" SSLEnabled="true">\n        <SSLHostConfig>\n            <Certificate certificateKeyAlias="tomcat" certificateKeystorePassword="changeme" certificateKeystoreFile="/etc/tomcat8/localhost-rsa.jks" type="RSA" />\n        </SSLHostConfig>\n    </Connector>\n\n    &@' ${TOMCAT_CONFIG}/server.xml
fi

sudo systemctl restart tomcat8
