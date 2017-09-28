#!/usr/bin/env bash

#
# Hootenanny Core Only configure and build script
#
# This script is basically the VagrantProvisionCentos7.sh script with the UI stuff removed. The intent is to
# have a script that can configure a machine and then build Hootenanny either using Vagrant or straight on
# an existing machine/VM
#
# VERY IMPORTANT NOTE: This script is designed to run in a very particular environment. Some of the directory
#   choices reflect this.
#
# You can run "vagrant up hoot_centos7_core" and you will get a Centos7 VM with just the Hootenanny core.
#
# To get this to build, we use a number of source and other packages outside of the normal core and EPEL repos.
# This script looks for the packages in the users home directory and if it can't find them, it tries to
# download them.
#
# The list of packages and their download locations are:
#
# Java8: jdk-8u144-linux-x64.rpm
# From: http://download.oracle.com/otn-pub/java/jdk/8u144-b01/090f390dda5b47b9b721c7dfaa008135/jdk-8u144-linux-x64.rpm
#
# NodeJS: nodejs-0.10.46 and nodejs-devel-0.10.46
# Installed via this repo: https://rpm.nodesource.com/setup
#
# STXXL: Release v1.3.1
# Pulled from GitHub: git clone http://github.com/stxxl/stxxl.git stxxl
#
# American-english-insane dictionary & words.sqlite. There isn't a package available for these so we use our own
# copy stored on S3
# https://s3.amazonaws.com/hoot-rpms/support-files/american-english-insane.bz2
# https://s3.amazonaws.com/hoot-rpms/support-files/words1.sqlite.bz2
#
# Alternate Maven mirror for packages
# https://repo.maven.apache.org/maven2
#
# Osmosis: osmosis-latest.tgz
# From: http://bretth.dev.openstreetmap.org/osmosis-build/osmosis-latest.tgz
#
# Postgresql95 from the Postgresql repo
# http://yum.postgresql.org/9.5/redhat/rhel-7-x86_64/pgdg-centos95-9.5-3.noarch.rpm
#
# GDAL gdal-2.1.4.tar.gz
# From: http://download.osgeo.org/gdal/2.1.4/gdal-2.1.4.tar.gz
#
# FileGDB API: FileGDB_API_1_5_1-64.tar.gz
# From: https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_1.5.1/FileGDB_API_1_5_1-64.tar.gz
#

# Setup some "local" config stuff. This directory has RPMs and source for things that have been already downloaded
# so we don't have to try to download them again.
# NOTE: This is _VERY_ site specific. DO NOT USE THIS UNLESS YOU _REALLY_ KNOW WHAT YOU ARE DOING
SRC_DIR=/home/centos/hoot-deps


VMUSER=`id -u -n`
echo USER: $VMUSER
VMGROUP=`groups | grep -o $VMUSER`
echo GROUP: $VMGROUP

# This is not optimal
HOOT_HOME=~/hoot
echo HOOT_HOME: $HOOT_HOME

cd ~
source ~/.bash_profile

# Keep VagrantBuild.sh happy
#ln -s ~/.bash_profile ~/.profile

# add EPEL repo for extra packages - ONLY if we DONT have SRC_DIR
if [ ! -d "$SRC_DIR" ] ; then
    echo "### Add epel repo ###" > CentOS_upgrade.txt
    sudo yum -y install epel-release >> CentOS_upgrade.txt 2>&1

    # This comes from the postgres repo
    POSTGIS_PACKAGE=postgis23_95

    # add the Postgres repo
    echo "### Add Postgres repo ###" > CentOS_upgrade.txt
    sudo rpm -Uvh http://yum.postgresql.org/9.5/redhat/rhel-7-x86_64/pgdg-centos95-9.5-3.noarch.rpm >> CentOS_upgrade.txt 2>&1
else
    echo "### Skipping the epel and postgresql repos..."

    # This one is from the site specific repo
    POSTGIS_PACKAGE=postgis2_95
fi

echo "Updating OS..."
echo "### Update ###" >> CentOS_upgrade.txt
sudo yum -q -y update >> CentOS_upgrade.txt 2>&1
echo "### Upgrade ###" >> CentOS_upgrade.txt
sudo yum -q -y upgrade >> CentOS_upgrade.txt 2>&1

echo "### Setup NTP..."
sudo yum -q -y install ntp
sudo chkconfig ntpd on
#TODO: Better way to do this?
sudo systemctl stop ntpd
sudo ntpd -gq
sudo systemctl start ntpd


# Make sure that we are in ~ before trying to wget & install stuff
cd ~

# add EPEL repo for extra packages - ONLY if we DONT have SRC_DIR
if [ ! -d "$SRC_DIR" ] ; then
    echo "### Installing the repo for an ancient version of NodeJS"
    curl --silent --location https://rpm.nodesource.com/setup | sudo bash -

    echo "### Installing an ancient version of NodeJS"
    sudo yum install -y \
        nodejs-0.10.46 \
        nodejs-devel-0.10.46
else
    # VERY HARDCODED
    echo "### Installing an ancient version of NodeJS"
    sudo yum install -y \
        $SRC_DIR/nodejs-0.10.46-1nodesource.el7.centos.x86_64.rpm \
        $SRC_DIR/nodejs-devel-0.10.46-1nodesource.el7.centos.x86_64.rpm
fi

# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
sudo yum -y install \
    asciidoc \
    autoconf \
    automake \
    bc \
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
    geos \
    geos-devel \
    git \
    git-core \
    glpk \
    glpk-devel \
    gnuplot \
    libicu-devel \
    libpng-devel \
    libtool \
    m4 \
    maven \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    perl-XML-LibXML \
    $POSTGIS_PACKAGE \
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
    qt \
    qt-devel \
    qt-postgresql \
    qtwebkit \
    qtwebkit-devel \
    sqlite \
    sqlite-devel \
    swig \
    tex* \
    unzip \
    v8-devel \
    w3m \
    wget \
    words \
    zip \


# Not installed:
#    xorg-x11-server-Xvfb \

# Official download page:
# http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
if  ! rpm -qa | grep jdk1.8.0_144-1.8.0_144; then
    echo "### Installing Java8..."
    if [ -f "$SRC_DIR/jdk-8u144-linux-x64.rpm" ]; then
      sudo yum -y install $SRC_DIR/jdk-8u144-linux-x64.rpm
    elif [ -f ./jdk-8u144-linux-x64.rpm ]; then
      sudo yum -y install ./jdk-8u144-linux-x64.rpm
    else
      JDKURL=http://download.oracle.com/otn-pub/java/jdk/8u144-b01/090f390dda5b47b9b721c7dfaa008135/jdk-8u144-linux-x64.rpm
      wget --quiet --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" $JDKURL
      sudo yum -y install ./jdk-8u144-linux-x64.rpm
    fi
fi

# Trying the following instead of removing the OpenJDK
# Setting /usr/java/jdk1.8.0_144/bin/java's priority to something really atrocious to guarantee that it will be
# the one used when alternatives' auto mode is used.
sudo alternatives --install /usr/bin/java java /usr/java/jdk1.8.0_144/bin/java 999999

# Setting /usr/java/jdk1.8.0_144/bin/javac's priority to something really atrocious to guarantee that it will be
# the one used when alternatives' auto mode is enabled.
sudo alternatives --install /usr/bin/javac javac /usr/java/jdk1.8.0_144/bin/javac 9999999

# switching to manual and forcing the desired version of java be configured
sudo alternatives --set java /usr/java/jdk1.8.0_144/bin/java

# switching to manual and forcing the desired version of javac be configured
sudo alternatives --set javac /usr/java/jdk1.8.0_144/bin/javac

# Now make sure that the version of Java we installed gets used.
# maven installs java-1.8.0-openjdk
#sudo rpm -e --nodeps java-1.8.0-openjdk-headless java-1.8.0-openjdk-devel java-1.8.0-openjdk

echo "##### Temp installs #####"

# Words1.sqlite is normally installed by Makefile.old
if [ -f "$SRC_DIR/words1.sqlite" ]; then
    echo "### Copying words1.sqlite..."
    cp $SRC_DIR/words1.sqlite $HOOT_HOME/conf/dictionary
fi

# Stxxl: We are expecting this to already be unpacked.
if [ -d "$SRC_DIR/stxxl" ] ; then
    cd $SRC_DIR/stxxl
else
    git clone http://github.com/stxxl/stxxl.git stxxl
    cd stxxl
    git checkout -q tags/1.3.1
fi
make config_gnu
echo "STXXL_ROOT	=`pwd`" > make.settings.local
echo "ENABLE_SHARED     = yes" >> make.settings.local
echo "COMPILER_GCC      = g++ -std=c++0x" >> make.settings.local
# Total hack because 1.3.1 doesn't compile right on CentOS7
sed -i 's/#include <sys\/mman.h>/#include <sys\/mman.h>\n#include <unistd.h>/g' ./utils/mlock.cpp

make -s library_g++

#### Isn't easy, no 'make install'
sudo install -p -D -m 0755 lib/libstxxl.so /usr/local/lib/libstxxl.so.1.3.1
sudo mkdir -p /usr/local/include
sudo cp -pr include/* /usr/local/include/
pushd .
cd /usr/local/lib
sudo ln -s libstxxl.so.1.3.1 libstxxl.so.1
sudo ln -s libstxxl.so.1.3.1 libstxxl.so
popd

sudo /sbin/ldconfig
#### So much easier to make later versions, uncomment when we upgrade to 1.4.0+
#mkdir build
#cd build
#cmake -DCMAKE_BUILD_TYPE=Release ..
#make -sj
#sudo make install -s

# Make sure we know where we are
cd ~

# Fix missing qmake
if ! hash qmake >/dev/null 2>&1 ; then
    if hash qmake-qt4 >/dev/null 2>&1 ; then
      sudo alternatives --install /usr/bin/qmake qmake /usr/bin/qmake-qt4 500
    else
      echo "##### No qmake! #####"
    fi
fi

# We need this big dictionary for text matching. On Ubuntu, this is a package
if [ ! -f /usr/share/dict/american-english-insane ]; then
    echo "### Installing american-english-insane dictionary..."
    if [ -f "$SRC_DIR/american-english-insane.bz2" ] ; then
        sudo bash -c "bzcat $SRC_DIR/american-english-insane.bz2 > /usr/share/dict/american-english-insane"
    elif  [ -f "$SRC_DIR/american-english-insane" ] ; then
        sudo bash -c "cp $SRC_DIR/american-english-insane /usr/share/dict/american-english-insane"
    elif [ -f ./american-english-insane.bz2 ] ; then
        sudo bash -c "bzcat ./american-english-insane.bz2 > /usr/share/dict/american-english-insane"
    else
        wget --quiet -N https://s3.amazonaws.com/hoot-rpms/support-files/american-english-insane.bz2
        sudo bash -c "bzcat american-english-insane.bz2 > /usr/share/dict/american-english-insane"
    fi
fi

#####
# Temp change until we get the C++11 support into develop
cd $HOOT_HOME
cp LocalConfig.pri.orig LocalConfig.pri
echo "QMAKE_CXXFLAGS += -std=c++11" >> LocalConfig.pri
#####


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
    echo "export JAVA_HOME=/usr/java/jdk1.8.0_144" >> ~/.bash_profile
    echo "export PATH=\$PATH:\$JAVA_HOME/bin" >> ~/.bash_profile
    source ~/.bash_profile
else
    sed -i '/^export JAVA_HOME=.*/c\export JAVA_HOME=\/usr\/java\/jdk1.8.0_144' ~/.bash_profile
fi

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

if [ ! -f bin/osmosis ]; then
    echo "### Installing Osmosis"
    mkdir -p ~/bin/osmosis_src

    if [ -f "$SRC_DIR/osmosis-latest.tgz" ]; then
      tar -zxf $SRC_DIR/osmosis-latest.tgz -C ~/bin/osmosis_src
    elif [ -f ./osmosis-latest.tgz ]; then
      tar -zxf ./osmosis-latest.tgz -C ~/bin/osmosis_src
    else
      wget --quiet http://bretth.dev.openstreetmap.org/osmosis-build/osmosis-latest.tgz
      tar -zxf osmosis-latest.tgz -C ~/bin/osmosis_src
    fi

    ln -s ~/bin/osmosis_src/bin/osmosis ~/bin/osmosis
fi

# Need to figure out a way to do this automagically
#PG_VERSION=$(sudo -u postgres psql -c 'SHOW SERVER_VERSION;' | egrep -o '[0-9]{1,}\.[0-9]{1,}'); do
PG_VERSION=9.5

if ! grep --quiet "psql-" ~/.bash_profile; then
    echo "Adding PostGres path vars to profile..."
    echo "export PATH=\$PATH:/usr/pgsql-${PG_VERSION}/bin" >> ~/.bash_profile
    source ~/.bash_profile
fi

if [ ! -f /etc/ld.so.conf.d/postgres${PG_VERSION}.conf ]; then
    sudo sh -c "echo '/usr/pgsql-${PG_VERSION}/lib' > /etc/ld.so.conf.d/postgres${PG_VERSION}.conf"
    sudo ldconfig
fi

# For convenience, set the version of GDAL and FileGDB to download and install
GDAL_VERSION=2.1.4
FGDB_VERSION=1.5.1
FGDB_VERSION2=`echo $FGDB_VERSION | sed 's/\./_/g;'`

if ! $( hash ogrinfo >/dev/null 2>&1 && ogrinfo --version | grep -q ${GDAL_VERSION} && ogrinfo --formats | grep -q FileGDB ); then
    if [ ! -d "gdal-${GDAL_VERSION}" ]; then
        if [ -f "$SRC_DIR/gdal-${GDAL_VERSION}.tar.gz" ]; then
            echo "### Extracting GDAL $GDAL_VERSION source..."
            tar zxfp $SRC_DIR/gdal-${GDAL_VERSION}.tar.gz
        elif [ -f "./gdal-${GDAL_VERSION}.tar.gz" ]; then
            echo "### Extracting GDAL $GDAL_VERSION source..."
            tar zxfp ./gdal-${GDAL_VERSION}.tar.gz
        else
            echo "### Downloading GDAL $GDAL_VERSION source..."
            wget --quiet http://download.osgeo.org/gdal/${GDAL_VERSION}/gdal-${GDAL_VERSION}.tar.gz
            echo "### Extracting GDAL $GDAL_VERSION source..."
            tar zxfp ./gdal-${GDAL_VERSION}.tar.gz
        fi
    fi # GDAL dir

    if [ ! -d /usr/local/FileGDB_API ]; then
        if [ -f "$SRC_DIR/FileGDB_API_${FGDB_VERSION2}-64.tar.gz" ]; then
            echo "### Extracting FileGDB API source & installing lib..."
            sudo mkdir -p /usr/local/FileGDB_API && sudo tar xfp $SRC_DIR/FileGDB_API_${FGDB_VERSION2}-64.tar.gz --directory /usr/local/FileGDB_API --strip-components 1
        elif [ ! -f "FileGDB_API_${FGDB_VERSION2}-64.tar.gz" ]; then
            echo "### Downloading FileGDB API source..."
            wget --quiet https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_${FGDB_VERSION}/FileGDB_API_${FGDB_VERSION2}-64.tar.gz
            echo "### Extracting FileGDB API source & installing lib..."
            sudo mkdir -p /usr/local/FileGDB_API && sudo tar xfp FileGDB_API_${FGDB_VERSION2}-64.tar.gz --directory /usr/local/FileGDB_API --strip-components 1
        fi
        sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
    fi

    echo "### Building GDAL $GDAL_VERSION w/ FileGDB..."
    export PATH=/usr/local/lib:/usr/local/bin:$PATH
    cd gdal-$GDAL_VERSION
    touch config.rpath
    echo "GDAL: configure"
    sudo ./configure --quiet --with-fgdb=/usr/local/FileGDB_API --with-pg=/usr/pgsql-${PG_VERSION}/bin/pg_config --with-python CFLAGS='-std=c11' CXXFLAGS='-std=c++11'
    echo "GDAL: make"
    sudo make -sj$(nproc) > GDAL_Build.txt 2>&1
    echo "GDAL: install"
    sudo make -s install >> GDAL_Build.txt 2>&1
    cd swig/python
    echo "GDAL: python build"
    python setup.py build >> GDAL_Build.txt 2>&1
    echo "GDAL: python install"
    sudo python setup.py install >> GDAL_Build.txt 2>&1
    sudo ldconfig
    cd ~
fi

if ! mocha --version &>/dev/null; then
    echo "### Installing mocha for plugins test..."
    sudo npm install --silent -g mocha
    # Clean up after the npm install
    sudo rm -rf ~/tmp
fi


echo "### Configureing Postgres..."
cd /tmp # Stop postgres "could not change directory to" warnings

# NOTE: These have been changed to pg9.5
# Postgresql startup
sudo /usr/pgsql-$PG_VERSION/bin/postgresql95-setup initdb
sudo systemctl start postgresql-$PG_VERSION
sudo systemctl enable postgresql-$PG_VERSION

# Get the configuration for the Database
source $HOOT_HOME/conf/database/DatabaseConfig.sh

# See if we already have a dB user
if ! sudo -u postgres psql -c "\du" | grep -iw --quiet $DB_USER; then
    echo "### Adding a Services Database user..."
    sudo -u postgres createuser --superuser $DB_USER
    sudo -u postgres psql -c "alter user $DB_USER with password '$DB_PASSWORD';"
fi

# Check that the OsmApiDb user exists
# NOTE:
#  + The OsmAPI Db user _might_ be different to the Hoot Services Db user...
#  + The SetupOsmApiDB.sh script expects that the DB_USER_OSMAPI account exists
if ! sudo -u postgres psql -c "\du" | grep -iw --quiet $DB_USER_OSMAPI; then
    sudo -u postgres createuser --superuser $DB_USER_OSMAPI
    sudo -u postgres psql -c "alter user $DB_USER_OSMAPI with password '$DB_PASSWORD_OSMAPI';"
fi


# Check for a hoot Db
if ! sudo -u postgres psql -lqt | grep -iw --quiet $DB_NAME; then
    echo "### Creating Services Database..."
    sudo -u postgres createdb $DB_NAME --owner=$DB_USER
    sudo -u postgres createdb wfsstoredb --owner=$DB_USER
    sudo -u postgres psql -d $DB_NAME -c 'create extension hstore;'
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'" > /dev/null
    sudo -u postgres psql -d wfsstoredb -c 'create extension postgis;' > /dev/null
fi

# configure Postgres settings
PG_HB_CONF=/var/lib/pgsql/$PG_VERSION/data/pg_hba.conf
if ! sudo -u postgres grep -i --quiet hoot $PG_HB_CONF; then
    sudo -u postgres cp $PG_HB_CONF $PG_HB_CONF.orig
    sudo -u postgres sed -i '1ihost    all            hoot            127.0.0.1/32            md5' $PG_HB_CONF
    sudo -u postgres sed -i '1ihost    all            hoot            ::1/128                 md5' $PG_HB_CONF
fi
POSTGRES_CONF=/var/lib/pgsql/$PG_VERSION/data/postgresql.conf
if ! sudo -u postgres grep -i --quiet HOOT $POSTGRES_CONF; then
    sudo -u postgres cp $POSTGRES_CONF $POSTGRES_CONF.orig
    sudo -u postgres sed -i s/^max_connections/\#max_connections/ $POSTGRES_CONF
    sudo -u postgres sed -i s/^shared_buffers/\#shared_buffers/ $POSTGRES_CONF
    sudo -u postgres bash -c "cat >> $POSTGRES_CONF" <<EOT
#--------------
# Hoot Settings
#--------------
max_connections = 1000
shared_buffers = 1024MB
max_files_per_process = 1000
work_mem = 16MB
maintenance_work_mem = 256MB
#checkpoint_segments = 20
autovacuum = off
EOT
fi

# configure kernel parameters
SYSCTL_CONF=/etc/sysctl.conf
if ! grep --quiet 1173741824 $SYSCTL_CONF; then
    sudo cp $SYSCTL_CONF $SYSCTL_CONF.orig
    echo "Setting kernel.shmmax"
    sudo sysctl -w kernel.shmmax=1173741824
    sudo sh -c "echo 'kernel.shmmax=1173741824' >> $SYSCTL_CONF"
    #                 kernel.shmmax=68719476736
fi
if ! grep --quiet 2097152 $SYSCTL_CONF; then
    echo "Setting kernel.shmall"
    sudo sysctl -w kernel.shmall=2097152
    sudo sh -c "echo 'kernel.shmall=2097152' >> $SYSCTL_CONF"
    #                 kernel.shmall=4294967296
fi
sudo systemctl restart postgresql-$PG_VERSION

# Get ready to build Hoot
echo "### Configuring Hoot..."
cd $HOOT_HOME
source ./SetupEnv.sh

# Not sure if we really need this...
if [ ! "$(ls -A hoot-ui)" ]; then
    echo "hoot-ui is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

if [ -f $HOOT_HOME/conf/LocalHoot.json ]; then
    echo "Removing LocalHoot.json..."
    rm -f $HOOT_HOME/conf/LocalHoot.json
fi

if [ -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf ]; then
    echo "Removing services local.conf..."
    rm -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf
fi

# Now build Hoot
source ./SetupEnv.sh
echo HOOT_HOME: $HOOT_HOME

# Going to remove this so that it gets updated
if [ -f missing ]; then
  rm -f missing
fi

aclocal && autoconf && autoheader && automake --add-missing --copy && ./configure --with-rnd --with-services

if [ ! -f LocalConfig.pri ] && ! grep --quiet QMAKE_CXX LocalConfig.pri; then
    echo 'Customizing LocalConfig.pri...'
    cp LocalConfig.pri.orig LocalConfig.pri
    sed -i s/"QMAKE_CXX=g++"/"#QMAKE_CXX=g++"/g LocalConfig.pri
    sed -i s/"#QMAKE_CXX=ccache g++"/"QMAKE_CXX=ccache g++"/g LocalConfig.pri
fi

echo "Building Hoot... "
echo "Will take several extra minutes to build the training data the initial time Hootenanny is installed only."
# Dropping this down by one as a test
# make -s clean && make -sj$(nproc)
make -s clean && make -sj3

echo "### Done..."
hoot version

##########################################
