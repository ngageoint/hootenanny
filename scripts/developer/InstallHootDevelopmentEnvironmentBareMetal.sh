#!/usr/bin/env bash

# This script is meant for installing an Ubuntu Hootenanny development enviroment to bare metal 
# for performance reasons (faster compiles than using vagrant on virtualbox with distcc).
#
# Pre-req:
#
# 1. Set up an Ubuntu 14.04 environment: http://mirror.pnl.gov/releases/14.04/
# 2. Create an ssh key and set up passwordless ssh locally.
# 3. Export your ssh key to Github: https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/
# 4. Install git.
# 5. Clone the hoot repo: git clone git@github.com:ngageoint/hootenanny.git hoot
# 6. OPTIONAL: If you are going to use the hoot-tests repo, have a DG admin create you an account on
# that server and add your ssh key to that repo.
# 7. OPTIONAL: Clone the hoot-tests repo. 
#
# Before running this script:
#
# 1. Temporarily change all references to '/home/vagrant' in VagrantProvision.sh to your home directory.
# 2. Temporarily change all references to the 'vagrant' user in VagrantProvision.sh to your user name.
# 3. sudo chmod 755 hoot/VagrantProvision.sh # Does this still have to be done?
#
# Run the script:
#
#  cd hoot
#  git submodule init && git submodule update
#  scripts/InstallHootDevelopmentEnvironment.sh
#
# This script:
#
#   - is one possible way to set up a Hootenanny developer environment on bare metal;  Feel free to tweak your own version of it.
#   - is not actively maintained and may occasionally need to be updated to work with the latest version of VagrantProvision.sh.
#   - calls into the vagrant ubuntu script for the first part of the installation.
#   - adds developer tools; Some of the tools installed (Eclipse, etc.) will require additional manual configuration.
#   - takes up roughly 13GB space (12/12/16; includes test output).
#   - sets up an environment which can work with as little as 4 GB of ram, but 8-16 GB is recommended for working with larger, real world 
#     datasets.
#   - does not handle setting up the hoot-tests repo, the translations repo, or the data used by hoot-tests (see Redmine docs for 
#     these steps).
#   - still needs to be updated to auto-handle some command prompts (TODO: fix)

cd ~

# Run all the default hoot vagrant scripts first, so we're working from the same baseline as the Vagrant users.

echo "Installing base dependencies from Vagrant provision script..."
hoot/VagrantProvision.sh
sleep 3

echo "Installing Hootenanny from Vagrant build script..."
hoot/VagrantBuild.sh
sleep 3

echo "Performing custom dev operations..."
sleep 3

HOOT_HOME=$HOME/hoot
echo HOOT_HOME: $HOOT_HOME

echo "Installing development environment dependencies..."

echo "Installing IDEs..."
# Unfortunately, the eclipse that ubuntu 14.04 comes with does not support tomcat 8, which hoot has now moved to.  This needs to
# be updated to either manually install a version of eclipse that does support tomcat 8 or wait until hoot upgrades to a newer 
# version of ubuntu which will likely install a version of eclipse that does integrate with tomcat 8.
sudo apt-get install -y qtcreator eclipse eclipse-jdt eclipse-pde eclipse-platform*
sleep 3

echo "Installing misc..."
# misc goodies - distcc requires further configuration
sudo apt-get install -y meld distcc htop synergy gparted kompare xclip
sleep 3

# TODO: This added repo will hose the geos install by exposing a conflict between opencv and 
# geos, so need to find a better way to install it.
#echo "Installing QGIS..."
#sudo apt-add-repository ppa:ubuntugis/ppa
#sudo apt-get update
#sudo apt-get install -y qgis
#sleep 3

echo "Installing JOSM..."
mkdir -p ~/local/jars/
wget -P ~/local/jars/ -N http://josm.openstreetmap.de/josm-tested.jar
# launch with:
#java -XX:MaxPermSize=1024m -Xmx8192m -jar ~/local/jars/josm-tested.jar
sleep 3

# needed for webex
#sudo apt-get install -y libpangoxft-1.0-0:i386 libxv1:i386 libpangox-1.0-0:i386 libgtk2.0-0 libxtst6 libxi-dev libasound2 libgtk2.0-0:i386 libxtst6:i386 libasound2:i386 libgcj14-awt:i386 icedtea-plugin

# If you're fed up with the unity desktop, then uncomment this for xfce; use 'xfce4-display-settings -m' after installing for display config
# sudo apt-get install xfce4

sudo apt-get autoremove -y

# Eclipse configuration - You will still additional Eclipse configuration (web server integration, extensions, etc.)  See Developer's Guide for details.
cd $HOOT_HOME
make eclipse  # generate eclipse project files
cd ~
if ! grep -i --quiet '-Xmx' /etc/eclipse.ini; then
  echo "Configuring Eclipse..."
sudo bash -c "cat >> /etc/eclipse.ini" <<EOT

-Dosgi.requiredJavaVersion=1.8
-XX:MaxPermSize=2048m
-Xms512m
-Xmx2048m
EOT
fi
if ! grep -i --quiet '--launcher.XXMaxPerSize' /etc/eclipse.ini; then
  echo "Configuring Eclipse..."
sudo bash -c "cat >> /etc/eclipse.ini" <<EOT

--launcher.XXMaxPermSize
2048m
EOT
fi
sleep 3

# Install vagrant/vbox here so that you can quickly spin up a different configurations of hoot if 
# needed, or troubleshoot the hoot vagrant install itself.
echo "Installing Vagrant..."
if [ ! -f vagrant_1.8.1_x86_64.deb ]; then
  wget https://releases.hashicorp.com/vagrant/1.8.1/vagrant_1.8.1_x86_64.deb
  sudo dpkg -i vagrant_1.8.1_x86_64.deb
  sudo apt-get -f install -y
fi
if [ ! -f virtualbox-5.0_5.0.14-105127~Ubuntu~trusty_amd64.deb ]; then
  wget http://download.virtualbox.org/virtualbox/5.0.14/virtualbox-5.0_5.0.14-105127~Ubuntu~trusty_amd64.deb
  sudo dpkg -i virtualbox-5.0_5.0.14-105127~Ubuntu~trusty_amd64.deb
  # Need to understand why this needs to be done...
  sudo rm -f /etc/apt/sources.list.d/virtualbox.list
  sudo apt-get -f install -y
fi

# uncomment this and edit the user name to get the hoot-tests data; this will take up to ~70GB
# of space at last check
# sudo mkdir -p /fouo/gis-data
# rsync -rzuv user.name@selma:/fouo/gis-data /fouo
