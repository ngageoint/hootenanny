#!/usr/bin/env bash

set -e

# The URL for the Hoot repo 
HOOTREPO="https://hoot-repo.s3.amazonaws.com/el7"

export LANG=en_US.UTF-8


echo "Adding additional software repositories..."
# add EPEL repo for extra packages
echo "### Add epel repo and handy packages ###" | tee CentOS_install.txt
sudo yum install -y wget curl sudo vim yum-utils epel-release 2>&1 | tee -a CentOS_install.txt

# add the repo for the current Hootenanny release rpm's.
echo "### Add Hoot repo ###" | tee -a CentOS_install.txt
sudo RUN yum-config-manager --add-repo $HOOTREPO/release/hoot.repo 2>&1 | tee -a CentOS_install.txt

# configure PGDG repository for PostgreSQL 9.5.
echo "### Add pgdg repo ###" | tee -a CentOS_install.txt
sudo yum-config-manager --add-repo $HOOTREPO/pgdg95.repo 2>&1 | tee -a CentOS_install.txt

if [ "${YUMUPDATE:-yes}" = "yes" ]; then
    echo "Updating OS..."
    echo "### Yum Upgrade ###" | tee -a CentOS_install.txt
    sudo yum -q -y upgrade 2>&1 | tee -a CentOS_install.txt
fi

if [ "${COREONLY:-yes}" = "yes" ]; then
    # Just the core
    echo "### Installing hootenanny-core ###" | tee -a CentOS_install.txt
    sudo yum install -y hootenanny-core 2>&1 | tee -a CentOS_install.txt
    echo "### Done ###" | tee -a CentOS_install.txt
else
    # Core, UI & services
    echo "### Add Hoot repo ###" | tee -a CentOS_install.txt
    sudo yum install -y hootenanny-autostart 2>&1 | tee -a CentOS_install.txt
    echo "### Done ###" | tee -a CentOS_install.txt
fi

# Sanity check
hoot version

echo "See VAGRANT.md for additional configuration instructions and then run 'vagrant ssh' to log into the Hootenanny virtual machine."
echo "See $HOOT_HOME/docs on the virtual machine for Hootenanny documentation files."
echo "Access the web application at http://localhost:8888/hootenanny-id"


# install useful and needed packages for working with hootenanny
# echo "### Installing handy packages repos..."
# sudo yum -y install \
#     bzip2 \
#     git \
#     git-core \
#     osmosis \
#     unzip \
#     vim \
#     wget \
#     zip

##########################################
