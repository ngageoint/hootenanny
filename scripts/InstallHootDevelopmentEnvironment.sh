#!/usr/bin/env bash

# This script:  
#
#   - is meant for installing hoot in a development enviroment to bare metal for performance reasons (instead of using the 
#     actively maintained Vagrant VM; faster compiles, etc.)
#   - is one possible way to set up a Hootenanny developer environment on bare metal;  Feel free to tweak your own version of it.
#   - is not actively maintained and may need to be updated to work with the latest version of VagrantProvision.sh
#   - calls into the vagrant ubuntu script for the first part of the installation
#   - adds developer tools; Some of the tools installed (Eclipse, etc.) will require additional manual configuration.
#   - takes up roughly 10GB space
#   - does not handle the hoot-tests repo, the translations repo, or the data used by hoot-tests (see Redmine docs for these steps)
#   - still needs to auto-handle some command prompts
#
# Pre-req:
#
# 1. Set up an Ubuntu 14.04 environment: http://mirror.pnl.gov/releases/14.04/
# 2. Export your ssh key to Github: https://help.github.com/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent/
# 3. Install git.
# 4. Clone the hoot repo: git clone git@github.com:ngageoint/hootenanny.git hoot
# 5. If you are going to use the hoot-tests repo, have a DG admin create you an account on
# that server and add your ssh key to that repo.
# 6. Clone the hoot-tests repo. 
#
# Before running this script:
#
# 1. Temporarily change all references to '/home/vagrant' in VagrantProvision.sh to your home directory.
# 2. Temporarily change all references to the 'vagrant' user in VagrantProvision.sh to your user.
# 3. sudo chmod 755 hoot/VagrantProvision.sh # Does this still have to be done?
#
# Run the script:
#
#  cd hoot
#  git submodule init && git submodule update
#  scripts/InstallHootDevelopmentEnvironment.sh

cd ~

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

# Use tomcat 7 here instead of tomcat 6, since eclipse installed from repos needs tomcat 7.
# TODO: This should go away after the upgrade to Tomcat 8.5.

echo "Backing up Tomcat 6 settings..."
mkdir -p ~/tmp
cp /etc/default/tomcat6 ~/tmp
cp /etc/tomcat6/server.xml ~/tmp
cp /etc/tomcat6/context.xml ~/tmp

echo "Removing Tomcat 6..."
sudo apt-get remove tomcat6
sudo apt-get autoremove -y

echo "Installing Tomcat 7..."
sudo apt-get install -y libtomcat7-java

echo "Restoring settings to Tomcat 7..."
sudo cp ~/tmp/tomcat6 /etc/default
sudo cp ~/tmp/server.xml /etc/tomcat7
sudo cp ~/tmp/context.xml /etc/tomcat7
# TODO: Do tomcat7 group permissions need to be set on these files in a developer environment?; Do sym links have to be made for the log dirs, etc?
sleep 3

echo "Installing development environment dependencies..."

echo "Installing IDEs..."
sudo apt-get install -y qtcreator eclipse eclipse-jdt eclipse-pde eclipse-platform*
sleep 3

echo "Installing misc..."
# misc goodies - distcc requires further configuration
sudo apt-get install -y meld distcc htop synergy gparted kompare xclip
sleep 3

# This added repo will hose the geos install by exposing a conflict between opencv and 
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
# TODO: there may still be some bugs with this part
cd $HOOT_HOME
make eclipse  # generate eclipse project files
cd ~
if ! grep -i --quiet '-Xmx' /etc/eclipse.ini; then
  echo "Configuring Eclipse..."
sudo bash -c "cat >> /etc/eclipse.ini" <<EOT

-Dosgi.requiredJavaVersion=1.7
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
