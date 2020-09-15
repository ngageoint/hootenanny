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

echo "Adding additional software repositories..."
echo "### Add epel repo and handy packages ###" | tee CentOS_install.txt
sudo yum install -y wget curl vim epel-release 2>&1 | tee -a CentOS_install.txt

# add the repo for the current Hootenanny release rpm's.
echo "### Add Hoot deps repo ###" | tee -a CentOS_install.txt
sudo $HOOT_HOME/scripts/yum/hoot-repo.sh

echo "### Add Hoot release repo ###" | tee -a CentOS_install.txt
sudo bash -c 'cat >> /etc/yum.repos.d/hoot_release.repo <<EOF
[hoot-release]
name = Hootenanny Release
baseurl = https://s3.amazonaws.com/hoot-repo/el7/release
enable = 1
gpgcheck = 1
repo_gpgcheck = 1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-Hoot
EOF'

# configure PGDG repository for PostgreSQL 9.5.
echo "### Add pgdg repo ###" | tee -a CentOS_install.txt
sudo $HOOT_HOME/scripts/yum/pgdg-repo.sh 9.5

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

echo "See VAGRANT.md for additional configuration instructions and then run 'vagrant ssh' to log into the Hootenanny virtual machine."
echo "See $HOOT_HOME/docs on the virtual machine for Hootenanny documentation files."
echo "If the webApp is installed, it is at http://localhost:8888/hootenanny-id"

##########################################
