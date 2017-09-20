#!/usr/bin/env bash

HOOT_HOME=~/hoot
TOMCAT_NAME=tomcat8
TOMCAT_GROUP=tomcat8
TOMCAT_USER=tomcat8
TOMCAT_HOME=/usr/share/tomcat8 # for binaries and other static files
TOMCAT_USER_HOME=/var/lib/tomcat8 # web applications go here
TOMCAT_CACHE_HOME=/var/cache/tomcat8 # temp and work folders go here
TOMCAT_LOGS_HOME=/var/log/tomcat8 # logs go here
TOMCAT_CONFIG_HOME=/etc/tomcat8 # config files go here
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Find out what user we have: vagrant or ubuntu
# This comes from a user difference in one of the Vagrant boxes
VMUSER=`id -u -n`
if [ "$VMUSER" = 'vagrant' ]
then
  TOMCAT_USER=$VMUSER
  TOMCAT_GROUP=$VMUSER
fi

#755 = -rwxr-xr-x
TOMCAT_HOME_PERMISSIONS=755
TOMCAT_USER_HOME_PERMISSIONS=755
TOMCAT_LOGS_HOME_PERMISSIONS=755
TOMCAT_CACHE_HOME_PERMISSIONS=775

TOMCAT_VERSION='8.5.20'
TOMCAT_TAR_FILE=${SCRIPT_HOME}/../apache-tomcat-${TOMCAT_VERSION}.tar.gz

echo "######## Begin $TOMCAT_NAME installation ########"
echo "SCRIPT_HOME=$SCRIPT_HOME"

# Stop tomcat service if it already exists
if sudo service --status-all 2>&1 | grep -q ${TOMCAT_NAME}; then
  sudo service ${TOMCAT_NAME} stop
fi

# Create tomcat group if not already created
sudo getent group ${TOMCAT_GROUP} >/dev/null || sudo groupadd -r ${TOMCAT_GROUP}

# Create tomcat user if not already created
sudo getent passwd ${TOMCAT_USER} >/dev/null || sudo useradd --comment "$TOMCAT_NAME daemon user" --shell /bin/bash -M -r -g ${TOMCAT_GROUP} --home ${TOMCAT_HOME} ${TOMCAT_USER}

# Add tomcat and ubuntu to each other's groups so we can get the group write working with nfs
if ! sudo groups $VMUSER | grep --quiet "\b$TOMCAT_GROUP\b"; then
    echo "### Adding $VMUSER user to $TOMCAT_GROUP user group..."
    sudo usermod -a -G ${TOMCAT_GROUP} ${VMUSER}
fi

# Add tomcat and ubuntu to each other's groups so we can get the group write working with nfs
if ! sudo groups ${TOMCAT_GROUP} | grep --quiet "\b$VMUSER\b"; then
    echo "### Adding $TOMCAT_GROUP user to $VMUSER user group..."
    sudo usermod -a -G ${VMUSER} ${TOMCAT_GROUP}
fi


sudo mkdir -p ${TOMCAT_HOME}
sudo tar -zxf ${TOMCAT_TAR_FILE} -C ${TOMCAT_HOME} --strip-components 1
sudo chmod -R ${TOMCAT_HOME_PERMISSIONS} ${TOMCAT_HOME}

sudo rm -rf ${TOMCAT_HOME}/webapps
sudo mkdir -p ${TOMCAT_USER_HOME}/webapps

sudo ln -sf ${TOMCAT_USER_HOME}/webapps ${TOMCAT_HOME}/webapps
sudo chmod ${TOMCAT_USER_HOME_PERMISSIONS} ${TOMCAT_USER_HOME}
sudo chown ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_USER_HOME}/webapps

#We don't need .bat files lying around
sudo rm -f ${TOMCAT_HOME}/bin/*.bat

echo "### Setting up Tomcat logs"
# Put logging in /var/log and link back.
sudo rm -rf ${TOMCAT_HOME}/logs
sudo mkdir -p ${TOMCAT_LOGS_HOME}
sudo chmod -R ${TOMCAT_LOGS_HOME_PERMISSIONS} ${TOMCAT_LOGS_HOME}
sudo chown ${TOMCAT_GROUP}:adm ${TOMCAT_LOGS_HOME}
sudo ln -sf ${TOMCAT_LOGS_HOME} ${TOMCAT_HOME}/logs

echo "### Done setting up Tomcat logs"

if [ -d "$TOMCAT_CONFIG_HOME" ]; then
    sudo rm -rf ${TOMCAT_CONFIG_HOME}
fi

sudo mkdir -p ${TOMCAT_CONFIG_HOME}

# Put conf in /etc/ and link back.
echo "### Setting up config"
sudo mkdir -p ${TOMCAT_HOME}/conf/policy.d
sudo cp ${SCRIPT_HOME}/../policy.d/* ${TOMCAT_HOME}/conf/policy.d
sudo mv ${TOMCAT_HOME}/conf/* ${TOMCAT_CONFIG_HOME}
sudo rm -rf ${TOMCAT_HOME}/conf
sudo ln -sf ${TOMCAT_CONFIG_HOME} ${TOMCAT_HOME}/conf
sudo mkdir -p ${TOMCAT_CONFIG_HOME}/Catalina
sudo chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CONFIG_HOME}/*
sudo chmod -R g+w ${TOMCAT_CONFIG_HOME}/*
echo "### Finished setting up config"

sudo mkdir -p ${TOMCAT_HOME}/.deegree
sudo chown ${TOMCAT_GROUP}:${TOMCAT_USER} ${TOMCAT_HOME}/.deegree

# Put temp and work to /var/cache and link back.
sudo mkdir -p ${TOMCAT_CACHE_HOME}

if [ -d "$TOMCAT_CACHE_HOME/temp" ]; then
    sudo rm -rf ${TOMCAT_CACHE_HOME}/temp
fi

sudo mv ${TOMCAT_HOME}/temp ${TOMCAT_CACHE_HOME}
sudo ln -sf ${TOMCAT_CACHE_HOME}/temp ${TOMCAT_HOME}/temp
sudo chmod ${TOMCAT_CACHE_HOME_PERMISSIONS} ${TOMCAT_CACHE_HOME}/temp
sudo chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CACHE_HOME}/temp

if [ -d "$TOMCAT_CACHE_HOME/work" ]; then
    sudo rm -rf ${TOMCAT_CACHE_HOME}/work
fi

sudo mv ${TOMCAT_HOME}/work ${TOMCAT_CACHE_HOME}
sudo ln -sf ${TOMCAT_CACHE_HOME}/work ${TOMCAT_HOME}/work
sudo chmod ${TOMCAT_CACHE_HOME_PERMISSIONS} ${TOMCAT_CACHE_HOME}/work
sudo chgrp -R ${TOMCAT_GROUP} ${TOMCAT_CACHE_HOME}/work

sudo cp ${SCRIPT_HOME}/etc/init.d/${TOMCAT_NAME} /etc/init.d
sudo cp ${SCRIPT_HOME}/etc/default/${TOMCAT_NAME} /etc/default
if [ "$TOMCAT_USER" != 'tomcat8' ]
then
  # replace the tomcat8 user and group with the user
  sudo sed -i "s/TOMCAT8_USER=tomcat8/TOMCAT8_USER=${TOMCAT_USER}/g" /etc/default/${TOMCAT_NAME}
  sudo sed -i "s/TOMCAT8_GROUP=tomcat8/TOMCAT8_GROUP=${TOMCAT_GROUP}/g" /etc/default/${TOMCAT_NAME}
  sudo sed -i "s/TOMCAT8_USER=tomcat8/TOMCAT8_USER=${TOMCAT_USER}/g" /etc/init.d/${TOMCAT_NAME}
  sudo sed -i "s/TOMCAT8_GROUP=tomcat8/TOMCAT8_GROUP=${TOMCAT_GROUP}/g" /etc/init.d/${TOMCAT_NAME}
fi

if ! grep -i --quiet 'ingest/processed' ${TOMCAT_CONFIG_HOME}/server.xml; then
    echo "Adding Tomcat context path for tile images..."
    sudo sed -i.bak 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/userfiles\/ingest\/processed\" path=\"\/static\" \/>\n      &@' ${TOMCAT_CONFIG_HOME}/server.xml
fi

if ! grep -i --quiet '<Resources allowLinking="true" />' ${TOMCAT_CONFIG_HOME}/context.xml; then
    echo "Set allowLinking to true in Tomcat context..."
    sudo sed -i.bak '/<Context>/a\    \<Resources allowLinking="true"/>' ${TOMCAT_CONFIG_HOME}/context.xml
fi

# Clean out tomcat logfile. We restart tomcat after provisioning.
sudo service ${TOMCAT_NAME} stop
sudo rm -f ${TOMCAT_LOGS_HOME}/catalina.out

sudo update-rc.d ${TOMCAT_NAME} defaults

echo "######## End $TOMCAT_NAME installation ########"
