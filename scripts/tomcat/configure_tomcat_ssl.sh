#!/usr/bin/env bash

set -e

# Auto-detect Tomcat version (support both Tomcat 8 and Tomcat 9)
if [ -d /etc/tomcat9 ]; then
    TOMCAT_CONFIG=/etc/tomcat9
    TOMCAT_SERVICE=tomcat9
elif [ -d /etc/tomcat8 ]; then
    TOMCAT_CONFIG=/etc/tomcat8
    TOMCAT_SERVICE=tomcat8
else
    echo "Error: Neither Tomcat 8 nor Tomcat 9 configuration found"
    exit 1
fi

echo "Configuring SSL for $TOMCAT_SERVICE..."

# OSM OAuth2 requires redirect URLs to use https
# Generate a keystore with a self-signed cert for localhost
if [ ! -f $TOMCAT_CONFIG/localhost-rsa.jks ]; then
sudo keytool -genkey -keyalg RSA -noprompt -alias tomcat -dname "CN=localhost, OU=NA, O=NA, L=NA, S=NA, C=NA" -keystore $TOMCAT_CONFIG/localhost-rsa.jks -validity 9999 -storepass changeme -keypass changeme
sudo chown root:tomcat $TOMCAT_CONFIG/localhost-rsa.jks
fi

# Enable SSL using self-signed cert
if ! grep -i --quiet 'certificateKeyAlias="tomcat"' ${TOMCAT_CONFIG}/server.xml; then
echo "Enabling SSL for Tomcat..."
sudo sed -i.bak 's@<!-- Define an AJP 1.3 Connector on port 8009 -->@<Connector port="8443" protocol="org.apache.coyote.http11.Http11NioProtocol" maxThreads="150" SSLEnabled="true">\n        <SSLHostConfig>\n            <Certificate certificateKeyAlias="tomcat" certificateKeystorePassword="changeme" certificateKeystoreFile="'"${TOMCAT_CONFIG}"'/localhost-rsa.jks" type="RSA" />\n        </SSLHostConfig>\n    </Connector>\n\n    &@' ${TOMCAT_CONFIG}/server.xml
fi

sudo systemctl restart $TOMCAT_SERVICE
