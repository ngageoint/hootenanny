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

# Ensure that CentOS Yum repository data is GPG-verified.
echo "### Configuring CentOS to verify repository metadata ###" | tee -a CentOS_install.txt
sudo yum-config-manager \
    --save \
    --setopt=base.repo_gpgcheck=1 \
    --setopt=extras.repo_gpgcheck=1 \
    --setopt=updates.repo_gpgcheck=1 &> /dev/null

# add EPEL repo for extra packages
echo "### Add epel repo ###" | tee -a CentOS_install.txt
sudo yum -y install epel-release 2>&1 | tee -a CentOS_install.txt

# add GEOINT for spatial libraries and utilities.
echo "### Add geoint-deps repo ###"  | tee -a CentOS_install.txt
sudo $HOOT_HOME/scripts/yum/geoint-repo.sh

# configure PGDG repository for PostgreSQL
echo "### Add pgdg repo ###"  | tee -a CentOS_install.txt
sudo $HOOT_HOME/scripts/yum/pgdg-repo.sh $POSTGRESQL_VERSION

# Now check if we should use the release or nightly (master) RPM's
# NOTE: The nightly RPM's are not signed
if [ "${NIGHTLY:-yes}" = "no" ]; then
    echo "### Adding the Hoot release repo ###" | tee -a CentOS_install.txt
    sudo yum-config-manager --add-repo https://hoot-repo.s3.amazonaws.com/el7/release/hoot.repo

else
    echo "### Adding the Hoot nightly master repo ###" | tee -a CentOS_install.txt
    sudo yum-config-manager --add-repo https://hoot-repo.s3.amazonaws.com/el7/master/hoot.repo
fi

if [ "${YUMUPDATE:-no}" = "yes" ]; then
    echo "Updating OS..."
    echo "### Yum Upgrade ###" | tee -a CentOS_install.txt
    sudo yum -q -y upgrade 2>&1 | tee -a CentOS_install.txt
fi

if [ "${COREONLY:-no}" = "yes" ]; then
    # Just the core
    echo "### Installing hootenanny-core ###" | tee -a CentOS_install.txt
    sudo yum install -y hootenanny-core 2>&1 | tee -a CentOS_install.txt
    echo "### Done ###" | tee -a CentOS_install.txt
else
    # Core, UI & services
    echo "### Installing Hootenanny-autostart ###" | tee -a CentOS_install.txt
    sudo yum install -y hootenanny-autostart 2>&1 | tee -a CentOS_install.txt
    echo "### Done ###" | tee -a CentOS_install.txt
fi

# Sanity check
hoot version

echo "### Configure OAuth redirect url for port 8888 ###" | tee -a CentOS_install.txt
sudo $HOOT_HOME/scripts/tomcat/configure_oauth_8888.sh


echo "See VAGRANT.md for additional configuration instructions and then run 'vagrant ssh' to log into the Hootenanny virtual machine."
echo "See $HOOT_HOME/docs on the virtual machine for Hootenanny documentation files."
echo "If the webApp is installed, it is at http://localhost:8888/hootenanny-id"

##########################################
