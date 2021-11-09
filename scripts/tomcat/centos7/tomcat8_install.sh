#!/bin/bash
set -euo pipefail

# Set HOOT_HOME to another location prior to running this script
# if ~/hoot isn't the correct location
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi
TOMCAT_NAME=tomcat8
TOMCAT_LEGACY_SYSTEMD=/etc/systemd/system/${TOMCAT_NAME}.service
TOMCAT_SYSTEMD=/usr/lib/systemd/system/${TOMCAT_NAME}.service
TOMCAT_LOGS=/var/log/tomcat8 # logs go here
TOMCAT_CONFIG=/etc/tomcat8 # config files go here

echo "######## Begin ${TOMCAT_NAME} installation ########"

# If old systemd service unit exists, then Tomcat was installed manually
# and must be uninstalled.
if test -f $TOMCAT_LEGACY_SYSTEMD; then
    # Disable, stop, and remove the previous systemd service.
    sudo systemctl disable tomcat8
    sudo systemctl stop tomcat8
    sudo rm -f $TOMCAT_LEGACY_SYSTEMD
    sudo systemctl daemon-reload

    # Clean up all manually-installed folders.
    sudo rm -fr /usr/share/tomcat8
    sudo rm -fr /var/lib/tomcat8
    sudo rm -fr /var/cache/tomcat8
    sudo rm -fr $TOMCAT_LOGS
    sudo rm -fr $TOMCAT_CONFIG
fi

# Install Tomcat from our package in Hootenanny's dependency repo.
sudo yum install -y tomcat8

# Modify the invoking user to be a part of the tomcat group, so it
# can write same directories as the tomcat service user.
sudo usermod -a -G tomcat $USER

# Add local configuration file that sets up environment variables
# for Hootenanny development.
sudo bash -c "cat >> ${TOMCAT_CONFIG}/conf.d/hoot.conf" << EOF
export GDAL_DATA=/usr/share/gdal
export HOOT_HOME=${HOOT_HOME}
export HOOT_WORKING_NAME=hootenanny
# This script is only used in the development environment vm
# that assumes a Java JDK is installed
export JAVA_HOME=/usr/lib/jvm/java-1.8.0
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib:$JAVA_HOME/jre/lib/amd64/server:${HOOT_HOME}/lib
export PATH=${HOOT_HOME}/bin:${PATH}
EOF

# Have the service run as the development user and group.
sudo sed -i "s/User=tomcat/User=${USER}/g" $TOMCAT_SYSTEMD
sudo sed -i "s/Group=tomcat/Group=${USER}/g" $TOMCAT_SYSTEMD

sudo systemctl daemon-reload
sudo systemctl enable $TOMCAT_NAME

if ! grep -i --quiet 'ingest/processed' ${TOMCAT_CONFIG}/server.xml; then
    echo "Adding Tomcat context path for tile images..."
    sudo sed -i.bak 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/userfiles\/ingest\/processed\" path=\"\/static\" \/>\n      &@' ${TOMCAT_CONFIG}/server.xml
fi

# Note: tomcat8 package already has `allowLinking=true` set in:
#  ${TOMCAT_CONFIG}/context.xml

# Clean out tomcat logfile. We restart tomcat after provisioning.
sudo systemctl stop $TOMCAT_NAME
sudo rm -f ${TOMCAT_LOGS}/catalina.out

# Recreate catalina.out to prevent error pop-up.
sudo bash -c "cat >> ${TOMCAT_LOGS}/catalina.out" <<EOF
Please login to the host to view the logs:

   sudo journalctl -u tomcat8
EOF
sudo chown $USER:$USER ${TOMCAT_LOGS}/catalina.out

echo "######## End ${TOMCAT_NAME} installation ########"
