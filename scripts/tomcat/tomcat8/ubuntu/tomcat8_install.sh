#!/usr/bin/env bash

HOOT_HOME=$HOME/hoot
TOMCAT_NAME=tomcat8
TOMCAT_GROUP=tomcat8
TOMCAT_USER=tomcat8
TOMCAT_HOME=/usr/share/tomcat8 # for binaries and other static files
TOMCAT_USER_HOME=/var/lib/tomcat8 # web applications go here
TOMCAT_CACHE_HOME=/var/cache/tomcat8 # temp and work folders go here
TOMCAT_LOGS_HOME=/var/log/tomcat8 # logs go here
TOMCAT_CONFIG_HOME=/etc/tomcat8 # config files go here
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#755 = -rwxr-xr-x

TOMCAT_HOME_PERMISSIONS=755
TOMCAT_USER_HOME_PERMISSIONS=755
TOMCAT_LOGS_HOME_PERMISSIONS=755
TOMCAT_CACHE_HOME_PERMISSIONS=775

TOMCAT_TAR_FILE=${SCRIPT_HOME}/../apache-tomcat-8.5.8.tar.gz

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi

echo "######## Begin $TOMCAT_NAME installation ########"
echo "SCRIPT_HOME=$SCRIPT_HOME"

# Stop tomcat service if it already exists
if service --status-all 2>&1 | grep -q ${TOMCAT_NAME}; then
  service ${TOMCAT_NAME} stop
fi

# Create tomcat group if not already created
getent group ${TOMCAT_GROUP} >/dev/null || groupadd -r ${TOMCAT_GROUP}

# Create tomcat user if not already created
getent passwd ${TOMCAT_USER} >/dev/null || useradd --comment "$TOMCAT_NAME daemon user" --shell /bin/bash -M -r -g ${TOMCAT_GROUP} --home ${TOMCAT_HOME} ${TOMCAT_USER}

# Add tomcat and vagrant to each other's groups so we can get the group write working with nfs
if ! groups vagrant | grep --quiet "\b$TOMCAT_GROUP\b"; then
    echo "### Adding vagrant user to $TOMCAT_GROUP user group..."
    usermod -a -G ${TOMCAT_GROUP} vagrant
fi

# Add tomcat and vagrant to each other's groups so we can get the group write working with nfs
if ! groups ${TOMCAT_GROUP} | grep --quiet "\bvagrant\b"; then
    echo "### Adding $TOMCAT_GROUP user to vagrant user group..."
    usermod -a -G vagrant ${TOMCAT_GROUP}
fi

mkdir -p ${TOMCAT_HOME}
tar -zxf ${TOMCAT_TAR_FILE} -C ${TOMCAT_HOME} --strip-components 1
chmod -R ${TOMCAT_HOME_PERMISSIONS} ${TOMCAT_HOME}

rm -rf ${TOMCAT_HOME}/webapps
mkdir -p ${TOMCAT_USER_HOME}/webapps

ln -sf ${TOMCAT_USER_HOME}/webapps ${TOMCAT_HOME}/webapps
chmod ${TOMCAT_USER_HOME_PERMISSIONS} ${TOMCAT_USER_HOME}
chown ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_USER_HOME}/webapps

#We don't need .bat files lying around
rm -f ${TOMCAT_HOME}/bin/*.bat

echo "### Setting up Tomcat logs"
# Put logging in /var/log and link back.
rm -rf ${TOMCAT_HOME}/logs
mkdir -p ${TOMCAT_LOGS_HOME}
chmod -R ${TOMCAT_LOGS_HOME_PERMISSIONS} ${TOMCAT_LOGS_HOME}
chown ${TOMCAT_GROUP}:adm ${TOMCAT_LOGS_HOME}
ln -sf ${TOMCAT_LOGS_HOME} ${TOMCAT_HOME}/logs

echo "### Done setting up Tomcat logs"

if [ -d "$TOMCAT_CONFIG_HOME" ]; then
    rm -rf ${TOMCAT_CONFIG_HOME}
fi

mkdir -p ${TOMCAT_CONFIG_HOME}

# Put conf in /etc/ and link back.
echo "### Setting up config"
mkdir -p ${TOMCAT_HOME}/conf/policy.d
cp ${SCRIPT_HOME}/../policy.d/* ${TOMCAT_HOME}/conf/policy.d
mv ${TOMCAT_HOME}/conf/* ${TOMCAT_CONFIG_HOME}
rm -rf ${TOMCAT_HOME}/conf
ln -sf ${TOMCAT_CONFIG_HOME} ${TOMCAT_HOME}/conf
mkdir -p ${TOMCAT_CONFIG_HOME}/Catalina
chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CONFIG_HOME}/*
chmod -R g+w ${TOMCAT_CONFIG_HOME}/*
echo "### Finished setting up config"

mkdir -p ${TOMCAT_HOME}/.deegree
chown ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_HOME}/.deegree

# Put temp and work to /var/cache and link back.
mkdir -p ${TOMCAT_CACHE_HOME}

if [ -d "$TOMCAT_CACHE_HOME/temp" ]; then
    rm -rf ${TOMCAT_CACHE_HOME}/temp
fi

mv ${TOMCAT_HOME}/temp ${TOMCAT_CACHE_HOME}
ln -sf ${TOMCAT_CACHE_HOME}/temp ${TOMCAT_HOME}/temp
chmod ${TOMCAT_CACHE_HOME_PERMISSIONS} ${TOMCAT_CACHE_HOME}/temp
chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CACHE_HOME}/temp

if [ -d "$TOMCAT_CACHE_HOME/work" ]; then
    rm -rf ${TOMCAT_CACHE_HOME}/work
fi

mv ${TOMCAT_HOME}/work ${TOMCAT_CACHE_HOME}
ln -sf ${TOMCAT_CACHE_HOME}/work ${TOMCAT_HOME}/work
chmod ${TOMCAT_CACHE_HOME_PERMISSIONS} ${TOMCAT_CACHE_HOME}/work
chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CACHE_HOME}/work

cp ${SCRIPT_HOME}/etc/init.d/${TOMCAT_NAME} /etc/init.d
cp ${SCRIPT_HOME}/etc/default/${TOMCAT_NAME} /etc/default

if ! grep -i --quiet 'ingest/processed' ${TOMCAT_CONFIG_HOME}/server.xml; then
    echo "Adding Tomcat context path for tile images..."
    sudo sed -i.bak 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/userfiles\/ingest\/processed\" path=\"\/static\" \/>\n      &@' ${TOMCAT_CONFIG_HOME}/server.xml
fi

if ! grep -i --quiet '<Resources allowLinking="true" />' ${TOMCAT_CONFIG_HOME}/context.xml; then
    echo "Set allowLinking to true in Tomcat context..."
    sudo sed -i.bak '/<Context>/a\    \<Resources allowLinking="true"/>' ${TOMCAT_CONFIG_HOME}/context.xml
fi

# Clean out tomcat logfile. We restart tomcat after provisioning.
service ${TOMCAT_NAME} stop
rm -f ${TOMCAT_LOGS_HOME}/catalina.out

update-rc.d ${TOMCAT_NAME} defaults

echo "######## End $TOMCAT_NAME installation ########"
