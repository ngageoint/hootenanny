#!/usr/bin/env bash

set -e

###################################################
# VERY IMPORTANT: Set the $HOOT_HOME environment  #
# variable prior to running this script if ~/hoot #
# isn't the correct location for HOOT_HOME        #
###################################################
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi
echo HOOT_HOME: $HOOT_HOME
#################################################

export LANG=en_US.UTF-8

# Common set of file versions
source $HOOT_HOME/VagrantProvisionVars.sh

echo "Adding software repositories..."

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

# add EPEL repo for extra packages
echo "### Add epel repo ###" | tee -a EL9_install.txt
sudo dnf -y install epel-release 2>&1 | tee -a EL9_install.txt

# add GEOINT for spatial libraries and utilities.
echo "### Add geoint-deps repo ###"  | tee -a EL9_install.txt
sudo $HOOT_HOME/scripts/yum/geoint-repo.sh

# configure PGDG repository for PostgreSQL
echo "### Add pgdg repo ###"  | tee -a EL9_install.txt
sudo $HOOT_HOME/scripts/yum/pgdg-repo.sh $POSTGRESQL_VERSION

# Now check if we should use the release or nightly (master) RPM's
# NOTE: The nightly RPM's are not signed
if [ "${NIGHTLY:-yes}" = "no" ]; then
    echo "### Adding the Hoot release repo ###" | tee -a EL9_install.txt
    sudo dnf config-manager --add-repo https://hoot-repo.s3.amazonaws.com/el9/release/hoot.repo

else
    echo "### Adding the Hoot nightly master repo ###" | tee -a EL9_install.txt
    sudo dnf config-manager --add-repo https://hoot-repo.s3.amazonaws.com/el9/master/hoot.repo
fi

if [ "${DNFUPDATE:-no}" = "yes" ]; then
    echo "Updating OS..."
    echo "### DNF Upgrade ###" | tee -a EL9_install.txt
    sudo dnf -q -y upgrade 2>&1 | tee -a EL9_install.txt
fi

if [ "${COREONLY:-no}" = "yes" ]; then
    # Just the core
    echo "### Installing hootenanny-core ###" | tee -a EL9_install.txt
    sudo dnf install -y hootenanny-core 2>&1 | tee -a EL9_install.txt
    echo "### Done ###" | tee -a EL9_install.txt
else
    # Core, UI & services
    echo "### Installing Hootenanny-autostart ###" | tee -a EL9_install.txt
    sudo dnf install -y hootenanny-autostart 2>&1 | tee -a EL9_install.txt
    echo "### Done ###" | tee -a EL9_install.txt
fi

# Sanity check
hoot version

echo "### Enable SSL for Tomcat ###" | tee -a EL9_install.txt
sudo $HOOT_HOME/scripts/tomcat/configure_tomcat_ssl.sh

echo "### Configure OAuth redirect url for port 8443 ###" | tee -a EL9_install.txt
sudo $HOOT_HOME/scripts/tomcat/configure_oauth_8443.sh


echo "See VAGRANT.md for additional configuration instructions and then run 'vagrant ssh' to log into the Hootenanny virtual machine."
echo "See $HOOT_HOME/docs on the virtual machine for Hootenanny documentation files."
echo "If the webApp is installed, it is at http://localhost:8443/hootenanny-id"

##########################################

