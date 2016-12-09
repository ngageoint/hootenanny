#!/usr/bin/env bash

HOOT_HOME=$HOME/hoot
TOMCAT_NAME=tomcat8
TOMCAT_GROUP=tomcat8
TOMCAT_USER=tomcat8
TOMCAT_HOME=/usr/share/tomcat8
TOMCAT_USER_HOME=/var/lib/tomcat8
TOMCAT_CACHE_HOME=/var/cache/tomcat8
TOMCAT_LOGS_HOME=/var/log/tomcat8
TOMCAT_CONFIG_HOME=/etc/tomcat8
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#Check boottime startup!

echo "### Installing Tomcat 8.5.8"

# Stop tomcat8 service if it exists
service tomcat8 stop

# Create tomcat8 group if not already created
getent group ${TOMCAT_GROUP} >/dev/null || groupadd -r ${TOMCAT_GROUP}

# Create tomcat8 user if not already created
getent passwd ${TOMCAT_USER} >/dev/null || useradd --comment "Tomcat 8 Daemon User" --shell /bin/bash -M -r -g ${TOMCAT_GROUP} --home ${TOMCAT_HOME} ${TOMCAT_USER}

# Add tomcat8 and vagrant to each others groups so we can get the group write working with nfs
if ! groups vagrant | grep --quiet "\b$TOMCAT_GROUP\b"; then
    echo "### Adding vagrant user to $TOMCAT_GROUP user group..."
    usermod -a -G ${TOMCAT_GROUP} vagrant
fi

# Add tomcat8 and vagrant to each others groups so we can get the group write working with nfs
if ! groups ${TOMCAT_GROUP} | grep --quiet "\bvagrant\b"; then
    echo "### Adding $TOMCAT_GROUP user to vagrant user group..."
    usermod -a -G vagrant ${TOMCAT_GROUP}
fi

echo "### Downloading Tomcat"
wget --quiet https://archive.apache.org/dist/tomcat/tomcat-8/v8.5.8/bin/apache-tomcat-8.5.8.tar.gz -P /tmp
echo "### Done downloading Tomcat"

mkdir -p ${TOMCAT_HOME}

echo "### Untarring Tomcat"
tar -zxf /tmp/apache-tomcat-8.5.8.tar.gz -C ${TOMCAT_HOME} --strip-components 1
echo "### Done untarring Tomcat"

rm -f /tmp/apache-tomcat-8.5.8.tar.gz

chmod -R 755 ${TOMCAT_HOME}

rm -rf ${TOMCAT_HOME}/webapps
mkdir -p ${TOMCAT_USER_HOME}/webapps

cd ${TOMCAT_HOME}

ln -sf ${TOMCAT_USER_HOME}/webapps webapps
chmod 775 ${TOMCAT_USER_HOME}
chown ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_USER_HOME}/webapps

cd -

#We don't need .bat file lying around
rm -f ${TOMCAT_HOME}/bin/*.bat

echo "### Setting up Tomcat logs"
# Put logging in /var/log and link back.
rm -rf ${TOMCAT_HOME}/logs
mkdir -p ${TOMCAT_LOGS_HOME}
chmod -R 755 ${TOMCAT_LOGS_HOME}
chown ${TOMCAT_GROUP}:adm ${TOMCAT_LOGS_HOME}
cd ${TOMCAT_HOME}

ln -sf ${TOMCAT_LOGS_HOME} logs

cd -
echo "### Done setting up Tomcat logs"

if [ ! -d "$TOMCAT_CONFIG_HOME" ]; then
    # Put conf in /etc/ and link back.
    echo "### Setting up config"
    mkdir -p ${TOMCAT_HOME}/conf/policy.d
    cp ${SCRIPT_HOME}/policy.d/* ${TOMCAT_HOME}/conf/policy.d
    mv ${TOMCAT_HOME}/conf ${TOMCAT_CONFIG_HOME}
    cd ${TOMCAT_HOME}
    ln -sf ${TOMCAT_CONFIG_HOME} conf
    mkdir -p ${TOMCAT_CONFIG_HOME}/Catalina
    chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CONFIG_HOME}/*
    chmod -R g+w ${TOMCAT_CONFIG_HOME}/*
    cd -
fi

# Put temp and work to /var/cache and link back.
mkdir -p ${TOMCAT_CACHE_HOME}
mkdir -p ${TOMCAT_HOME}/.deegree
chown ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_HOME}/.deegree

if [ ! -d "$TOMCAT_CACHE_HOME/temp" ]; then
    mv ${TOMCAT_HOME}/temp ${TOMCAT_CACHE_HOME}
fi

if [ ! -d "$TOMCAT_CACHE_HOME/work" ]; then
    mv ${TOMCAT_HOME}/work ${TOMCAT_CACHE_HOME}
fi

cd ${TOMCAT_HOME}

ln -sf ${TOMCAT_CACHE_HOME}/temp temp
ln -sf ${TOMCAT_CACHE_HOME}/work work
chmod 775 ${TOMCAT_CACHE_HOME}/temp
chmod 775 ${TOMCAT_CACHE_HOME}/work
chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CACHE_HOME}/temp
chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CACHE_HOME}/work

cd -

# Trying this to try to get rid of errors
mkdir -p ${TOMCAT_HOME}/server/classes
mkdir -p ${TOMCAT_HOME}/shared/classes
chown -R ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_HOME}/server
chown -R ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_HOME}/shared

cp ${SCRIPT_HOME}/etc/init.d/${TOMCAT_NAME} /etc/init.d
cp ${SCRIPT_HOME}/etc/default/${TOMCAT_NAME} /etc/default

if ! grep -i --quiet 'ingest/processed' ${TOMCAT_CONFIG_HOME}/server.xml; then
    echo "Adding Tomcat context path for tile images..."
    sudo sed -i.bak 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/ingest\/processed\" path=\"\/static\" \/>\n      &@' ${TOMCAT_CONFIG_HOME}/server.xml
fi

if ! grep -i --quiet 'allowLinking="true"' ${TOMCAT_CONFIG_HOME}/context.xml; then
    echo "Set allowLinking to true in Tomcat context..."
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" ${TOMCAT_CONFIG_HOME}/context.xml
fi

# Clean out tomcat logfile. We restart tomcat after provisioning.
service ${TOMCAT_NAME} stop
rm -f ${TOMCAT_LOGS_HOME}/catalina.out

sudo update-rc.d ${TOMCAT_NAME} defaults