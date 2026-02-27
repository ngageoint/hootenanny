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

# Common set of file versions
source $HOOT_HOME/VagrantProvisionVars.sh

VMUSER=`id -u -n`
echo USER: $VMUSER
VMGROUP=`groups | grep -o $VMUSER`
echo GROUP: $VMGROUP

export LANG=en_US.UTF-8

# Make sure that we are in ~ before trying to wget & install stuff
cd ~
source ~/.bash_profile

if [ "${ADDREPOS:-yes}" = "yes" ]; then
    echo "Adding additional software repositories..."

    # Ensure that EL9 DNF repository data is GPG-verified.
    echo "### Configuring EL9 to verify repository metadata ###" >> EL9_upgrade.txt
    sudo dnf config-manager \
        --save \
        --setopt=baseos.repo_gpgcheck=1 \
        --setopt=appstream.repo_gpgcheck=1 &> /dev/null

    # add EPEL repo for extra packages
    echo "### Add epel repo ###" >> EL9_upgrade.txt
    sudo dnf -y install epel-release >> EL9_upgrade.txt 2>&1

    # add GEOINT for spatial libraries and utilities.
    echo "### Add geoint-deps repo ###" >> EL9_upgrade.txt
    sudo $HOOT_HOME/scripts/yum/geoint-repo.sh

    # add Hoot repo for our pre-built dependencies.
    echo "### Add Hoot repo ###" >> EL9_upgrade.txt
    sudo $HOOT_HOME/scripts/yum/hoot-repo.sh

    # configure PGDG repository for PostgreSQL
    echo "### Add pgdg repo ###" >> EL9_upgrade.txt
    sudo $HOOT_HOME/scripts/yum/pgdg-repo.sh $POSTGRESQL_VERSION
fi

# Install PostgreSQL libraries (no devtoolset needed on EL9)
echo "### Installing postgresql ${POSTGRESQL_VERSION} libraries ###"
sudo dnf install -y postgresql${POSTGRESQL_VERSION}-libs >> EL9_upgrade.txt 2>&1

if [ "${DNFUPDATE:-yes}" = "yes" ]; then
    echo "Updating OS..."
    echo "### DNF Upgrade ###" >> EL9_upgrade.txt
    echo "dnf upgrade"
    sudo dnf -q -y upgrade # >> EL9_upgrade.txt 2>&1
    echo "dnf worked"
fi

# Install hootenanny dependencies
$HOOT_HOME/VagrantProvisionEL9Deps.sh

# Fix missing qmake
if ! hash qmake >/dev/null 2>&1 ; then
    if hash qmake-qt5 >/dev/null 2>&1 ; then
      sudo alternatives --install /usr/bin/qmake qmake /usr/bin/qmake-qt5 500
    else
      echo "##### No qmake! #####"
    fi
fi

cd $HOOT_HOME

echo "### Configuring environment..."

# Configure https alternative mirror for maven install
sudo /usr/bin/perl $HOOT_HOME/scripts/maven/SetMavenHttps.pl

if ! grep --quiet "export HOOT_HOME" ~/.bash_profile; then
    echo "Adding hoot home to profile..."
    echo "export HOOT_HOME=$HOOT_HOME" >> ~/.bash_profile
    echo "export PATH=\$PATH:\$HOOT_HOME/bin" >> ~/.bash_profile
    source ~/.bash_profile
fi

if ! grep --quiet "GDAL_DATA" ~/.bash_profile; then
    echo "Adding GDAL data path to profile..."
    echo "export GDAL_DATA=/usr/share/gdal" >> ~/.bash_profile
    source ~/.bash_profile
fi

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

# Install Google Chrome and ChromeDriver.
$HOOT_HOME/scripts/chrome/driver-install.sh

# Configure PostgreSQL
echo "### Configuring PostgreSQL..."
$HOOT_HOME/scripts/database/ConfigurePostgresql.sh

echo "### Creating databases..."
$HOOT_HOME/scripts/database/SetupHootDb.sh

# Get ready to build Hoot
echo "SetupEnv.sh"
cd $HOOT_HOME
echo "$HOOT_HOME"
echo `pwd`
source ./SetupEnv.sh

if [ ! "$(ls -A hoot-ui-2x)" ]; then
    echo "hoot-ui-2x is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

echo "### Installing Tomcat9..."
TOMCAT_HOME=/usr/share/tomcat9

# Install Tomcat 9 (EL9 uses Tomcat 9)
$HOOT_HOME/scripts/tomcat/el9/tomcat9_install.sh

# Enable SSL for Tomcat
$HOOT_HOME/scripts/tomcat/configure_tomcat_ssl.sh

# Configure Tomcat for the user
if ! grep --quiet TOMCAT9_HOME ~/.bash_profile; then
    echo "### Adding Tomcat to profile..."
    echo "export TOMCAT9_HOME=$TOMCAT_HOME" >> ~/.bash_profile
    source ~/.bash_profile
fi

if [ -f $HOOT_HOME/conf/LocalHoot.json ]; then
    echo "Removing LocalHoot.json..."
    rm -f $HOOT_HOME/conf/LocalHoot.json
fi

if [ -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf ]; then
    echo "Removing services local.conf..."
    rm -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf
fi

# Making sure we know where we are
cd ~
echo "### Installing node-export-server..."
sudo cp $HOOT_HOME/node-export-server/systemd/node-export.service /etc/systemd/system/node-export.service
sudo sed -i "s|SERVICE_USER|$VMUSER|g" /etc/systemd/system/node-export.service
sudo sed -i "s|HOOTENANNY_HOME|$HOOT_HOME|g" /etc/systemd/system/node-export.service
# Make sure all npm modules are installed
cd $HOOT_HOME/node-export-server
npm install --silent
# Clean up after the npm install
rm -rf ~/tmp

# Notify systemd that unit files have changed.
sudo systemctl daemon-reload

cd $HOOT_HOME

echo "### Hootenanny EL9 provisioning complete! ###"
echo "Run 'vagrant provision' with the build provisioner to compile Hootenanny."

