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
    # add EPEL repo for extra packages
    echo "### Add epel repo ###" > CentOS_upgrade.txt
    sudo yum -y install epel-release >> CentOS_upgrade.txt 2>&1

    # add Hoot repo for our pre-built dependencies.
    echo "### Add Hoot repo ###" >> CentOS_upgrade.txt
    sudo $HOOT_HOME/scripts/yum/hoot-repo.sh

    # configure PGDG repository for PostgreSQL 9.5.
    echo "### Add pgdg repo ###" >> CentOS_upgrade.txt
    sudo $HOOT_HOME/scripts/yum/pgdg-repo.sh 9.5
fi

if [ "${YUMUPDATE:-yes}" = "yes" ]; then
    echo "Updating OS..."
    echo "### Yum Upgrade ###" >> CentOS_upgrade.txt
    sudo yum -q -y upgrade >> CentOS_upgrade.txt 2>&1
fi

if ! rpm -qa | grep -q ^yum-plugin-versionlock ; then
    # Install the versionlock plugin version first.
    sudo yum install -y yum-plugin-versionlock
else
    # Remove any version locks to allow upgrading when versions have changed.
    sudo yum versionlock delete \
         armadillo \
         geos \
         geos-devel \
         glpk \
         glpk-devel \
         hoot-gdal \
         hoot-gdal-devel \
         hoot-gdal-python \
         libgeotiff \
         libgeotiff-devel \
         liboauthcpp \
         liboauthcpp-devel \
         libphonenumber \
         libphonenumber-devel \
         nodejs \
         nodejs-devel \
         proj \
         proj-devel \
         stxxl \
         stxxl-devel

fi

echo "### Installing libraries with locked versions"
sudo yum install -y \
     armadillo-$ARMADILLO_VERSION \
     geos-$GEOS_VERSION \
     geos-devel-$GEOS_VERSION \
     glpk-$GLPK_VERSION \
     glpk-devel-$GLPK_VERSION \
     hoot-gdal-$GDAL_VERSION \
     hoot-gdal-devel-$GDAL_VERSION \
     hoot-gdal-python-$GDAL_VERSION \
     libgeotiff-$LIBGEOTIFF_VERSION \
     libgeotiff-devel-$LIBGEOTIFF_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     nodejs-$NODE_VERSION \
     nodejs-devel-$NODE_VERSION \
     proj-$PROJ_VERSION \
     proj-devel-$PROJ_VERSION \
     stxxl-$STXXL_VERSION \
     stxxl-devel-$STXXL_VERSION

echo "### Locking versions of libraries"
sudo yum versionlock add \
     armadillo-$ARMADILLO_VERSION \
     geos-$GEOS_VERSION \
     geos-devel-$GEOS_VERSION \
     glpk-$GLPK_VERSION \
     glpk-devel-$GLPK_VERSION \
     hoot-gdal-$GDAL_VERSION \
     hoot-gdal-devel-$GDAL_VERSION \
     hoot-gdal-python-$GDAL_VERSION \
     libgeotiff-$LIBGEOTIFF_VERSION \
     libgeotiff-devel-$LIBGEOTIFF_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     nodejs-$NODE_VERSION \
     nodejs-devel-$NODE_VERSION \
     proj-$PROJ_VERSION \
     proj-devel-$PROJ_VERSION \
     stxxl-$STXXL_VERSION \
     stxxl-devel-$STXXL_VERSION

# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
sudo yum -y install \
    asciidoc \
    autoconf \
    autoconf-archive \
    automake \
    boost-devel \
    bzip2 \
    ccache \
    cmake \
    cppunit-devel \
    dblatex \
    doxygen \
    gcc \
    gcc-c++ \
    gdb \
    git \
    git-core \
    gnuplot \
    lcov \
    libicu-devel \
    libpng-devel \
    libtool \
    m4 \
    maven \
    mlocate \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    osmosis \
    java-1.8.0-openjdk \
    perl-XML-LibXML \
    hoot-postgis24_95 \
    libpostal-data \
    libpostal-devel \
    postgresql95 \
    postgresql95-contrib \
    postgresql95-devel \
    postgresql95-server \
    proj \
    proj-devel \
    protobuf \
    protobuf-compiler \
    protobuf-devel \
    python  \
    python-devel \
    python-matplotlib \
    python-pip  \
    python-setuptools \
    qt5-qtbase \
    qt5-qtbase-devel \
    qt5-qtbase-postgresql \
    qt5-qtwebkit \
    qt5-qtwebkit-devel \
    redhat-lsb-core \
    swig \
    tex-fonts-hebrew \
    texlive \
    texlive-collection-fontsrecommended \
    texlive-collection-langcyrillic \
    unzip \
    vim \
    wamerican-insane \
    w3m \
    wget \
    words \
    xorg-x11-server-Xvfb \
    zip \

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

# Configure https alternative mirror for maven install, this can likely be removed once
# we are using maven 3.2.3 or higher
sudo /usr/bin/perl $HOOT_HOME/scripts/maven/SetMavenHttps.pl

if ! grep --quiet "export HOOT_HOME" ~/.bash_profile; then
    echo "Adding hoot home to profile..."
    echo "export HOOT_HOME=$HOOT_HOME" >> ~/.bash_profile
    echo "export PATH=\$PATH:\$HOOT_HOME/bin" >> ~/.bash_profile
    source ~/.bash_profile
fi

if ! grep --quiet "export JAVA_HOME" ~/.bash_profile; then
    echo "Adding Java home to profile..."
    echo "export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk" >> ~/.bash_profile
    echo "export PATH=\$PATH:\$JAVA_HOME/bin" >> ~/.bash_profile
    source ~/.bash_profile
else
    sed -i '/^export JAVA_HOME=.*/c\export JAVA_HOME=\/usr\/lib\/jvm\/java-1.8.0-openjdk' ~/.bash_profile
fi

# Update the GDAL_DATA folder in ~/.bash_profile
if ! grep --quiet GDAL_DATA ~/.bash_profile; then
    echo "Adding GDAL data path to profile..."
    echo "export GDAL_DATA=$(gdal-config --datadir)" >> ~/.bash_profile
    source ~/.bash_profile
fi

# Use RVM to install the desired Ruby version, and then install
# the bundler at the desired version.
$HOOT_HOME/scripts/ruby/rvm-install.sh
$HOOT_HOME/scripts/ruby/bundler-install.sh

# Install gems with bundler and strict versioning (see Gemfile)
$RVM_HOME/bin/rvm $RUBY_VERSION_HOOT do bundle install

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

# Install Google Chrome and ChromeDriver.
$HOOT_HOME/scripts/chrome/chrome-install.sh
$HOOT_HOME/scripts/chrome/driver-install.sh

# Configure PostgreSQL
echo "### Configuring PostgreSQL..."
$HOOT_HOME/scripts/database/ConfigurePostgresql.sh

echo "### Createing databases..."
$HOOT_HOME/scripts/database/SetupHootDb.sh

if ! mocha --version &>/dev/null; then
    echo "### Installing mocha for translations test..."
    sudo npm install --silent -g mocha@3.5.3
    # Clean up after the npm install
    sudo rm -rf ~/tmp
fi

# Get ready to build Hoot
echo "SetupEnv.sh"
cd $HOOT_HOME
echo "$HOOT_HOME"
echo `pwd`
source ./SetupEnv.sh

if [ ! "$(ls -A hoot-ui)" ]; then
    echo "hoot-ui is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

echo "### Installing Tomcat8..."
TOMCAT_HOME=/usr/share/tomcat8

# Install Tomcat 8
$HOOT_HOME/scripts/tomcat/centos7/tomcat8_install.sh

# Configure Tomcat for the user
if ! grep --quiet TOMCAT8_HOME ~/.bash_profile; then
    echo "### Adding Tomcat to profile..."
    echo "export TOMCAT8_HOME=$TOMCAT_HOME" >> ~/.bash_profile
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

# Update marker file date now that dependency and config stuff has run
# The make command will exit and provide a warning to run 'vagrant provision'
# if the marker file is older than this file (VagrantProvision.sh)
touch Vagrant.marker

# Setup and clean out the directories that the UI uses
if [ ! -d "$HOOT_HOME/userfiles/ingest/processed" ]; then
    mkdir -p $HOOT_HOME/userfiles/ingest/processed
fi

# wipe out all dirs. tmp and upload now reside under $HOOT_HOME/userfiles/
rm -rf $HOOT_HOME/upload
rm -rf $HOOT_HOME/tmp

if [ -d "$HOOT_HOME/data/reports" ]; then
    echo "Moving contents of $HOOT_HOME/data/reports to $HOOT_HOME/userfiles/"
    cp -R $HOOT_HOME/data/reports $HOOT_HOME/userfiles/
    rm -rf $HOOT_HOME/data/reports
fi

if [ -d "$HOOT_HOME/customscript" ]; then
    echo "Moving contents of $HOOT_HOME/customscript to $HOOT_HOME/userfiles/"
    cp -R $HOOT_HOME/customscript $HOOT_HOME/userfiles/
    rm -rf $HOOT_HOME/customscript
fi

if [ -d "$HOOT_HOME/ingest" ]; then
    echo "Moving contents of $HOOT_HOME/ingest to $HOOT_HOME/userfiles/"
    cp -R $HOOT_HOME/ingest $HOOT_HOME/userfiles/
    rm -rf $HOOT_HOME/ingest
fi

# Always start with a clean $HOOT_HOME/userfiles/tmp
rm -rf $HOOT_HOME/userfiles/tmp

# This is defensive!
# We do this so that Tomcat doesnt. If it does, it screws the permissions up
mkdir -p $HOOT_HOME/userfiles/tmp

# OK, this is seriously UGLY but it fixes an NFS problem
#chmod -R 777 $HOOT_HOME/userfiles

# This is very ugly.
# If we don't have access to the directory where HOOT_HOME is, Tomcat chokes
#chmod go+rx ~


##########################################
