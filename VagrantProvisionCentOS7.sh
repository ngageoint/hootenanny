#!/usr/bin/env bash


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

# Centos7 specific file versions
export STXXL_VERSION=stxxl-1.3.1

export LANG=en_US.UTF-8

cd ~
source ~/.bash_profile

# add EPEL repo for extra packages
echo "### Add epel repo ###" > CentOS_upgrade.txt
sudo yum -y install epel-release >> CentOS_upgrade.txt 2>&1

# add the Postgres repo
echo "### Add Postgres repo ###" >> CentOS_upgrade.txt
sudo rpm -Uvh https://download.postgresql.org/pub/repos/yum/9.5/redhat/rhel-7-x86_64/pgdg-centos95-9.5-3.noarch.rpm  >> CentOS_upgrade.txt 2>&1

echo "Updating OS..."
echo "### Update ###" >> CentOS_upgrade.txt
sudo yum -q -y update >> CentOS_upgrade.txt 2>&1
echo "### Upgrade ###" >> CentOS_upgrade.txt
sudo yum -q -y upgrade >> CentOS_upgrade.txt 2>&1
echo "### Reinstall libc-common ###" >> CentOS_upgrade.txt 2>&1
sudo yum -q -y reinstall glibc-common >> CentOS_upgrade.txt 2>&1
sudo localedef -i en_US -f UTF-8 en_US.UTF-8

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

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
    libpng-devel \
    libtool \
    m4 \
    maven \
    mlocate \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    java-1.8.0-openjdk \
    perl-XML-LibXML \
    postgis23_95 \
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
    redhat-lsb-core \
    swig \
    tex-fonts-hebrew \
    texlive \
    texlive-collection-fontsrecommended \
    texlive-collection-langcyrillic \
    unzip \
    vim \
    w3m \
    wget \
    words \
    xorg-x11-server-Xvfb \
    zip \


echo "##### Temp installs #####"

# ICU:
echo "##### ICU #####"
cd ~
wget --quiet http://download.icu-project.org/files/icu4c/57.1/icu4c-57_1-src.tgz
tar xzf icu4c-57_1-src.tgz
cd icu/source/
CXXFLAGS=-std=c++11 ./runConfigureICU Linux/gcc > ~/centos7_install.txt
gmake -sj$(nproc) >> ~/centos7_install.txt
sudo gmake -s install >> ~/centos7_install.txt

# Node
echo "##### NodeJs #####"
NODE_VERSION=8.9.0
# Install the binary version of NodeJs for some uses (including npm)
cd ~
wget --quiet https://nodejs.org/dist/v${NODE_VERSION}/node-v${NODE_VERSION}-linux-x64.tar.xz
tar xf node-v${NODE_VERSION}-linux-x64.tar.xz
cd node-v${NODE_VERSION}-linux-x64
sudo cp -r bin /usr/
sudo cp -r include /usr/
sudo cp -r lib /usr/
sudo cp -r share /usr/
# Build the shared library version of NodeJs for hootenanny
cd ~
wget --quiet https://nodejs.org/dist/v${NODE_VERSION}/node-v${NODE_VERSION}.tar.gz
tar xf node-v${NODE_VERSION}.tar.gz
cd node-v${NODE_VERSION}
./configure --shared --prefix=/usr >> ~/centos_install.txt
make -j$(nproc) >> ~/centos_install.txt
sudo make install >> ~/centos_install.txt
sudo ln -s /usr/lib/libnode.so.57 /usr/lib/libnode.so

# Google depot tools
#echo "##### Depot Tools #####"
#cd ~
#git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git >> ~/centos7_install.txt
#export PATH=$PATH:~/depot_tools

# V8
#echo "##### V8 #####"
#cd ~
#fetch v8 >> ~/centos7_install.txt
#cd v8
#git checkout 5.1-lkgr >> ~/centos7_install.txt
#gclient sync >> ~/centos7_install.txt
#make -sj$(nproc) x64.release library=shared use_system_icu=1 >> ~/centos7_install.txt
# i18nsupport=off
#sudo cp out/x64.release/lib.target/*.so /usr/lib64/
#sudo cp include/* /usr/include/node/
#sudo cp -r include/libplatform /usr/include/node/

# Stxxl
cd ~
if [ ! -f "${STXXL_VERSION}.tar.gz" ]; then
    wget --quiet https://github.com/ngageoint/hootenanny-rpms/raw/master/src/SOURCES/${STXXL_VERSION}.tar.gz
fi
mkdir -p stxxl && tar zxof ${STXXL_VERSION}.tar.gz --directory ./stxxl --strip-components 1

cd stxxl
make -s config_gnu
echo "STXXL_ROOT	=`pwd`" > make.settings.local
echo "ENABLE_SHARED     = yes" >> make.settings.local
echo "COMPILER_GCC      = g++ -std=c++11" >> make.settings.local
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

cd ~
#### So much easier to make later versions, uncomment when we upgrade to 1.4.0+
#mkdir build
#cd build
#cmake -DCMAKE_BUILD_TYPE=Release ..
#make -sj
#sudo make install -s

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
    if [ -f ./american-english-insane.bz2 ] ; then
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
    echo "export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk" >> ~/.bash_profile
    echo "export PATH=\$PATH:\$JAVA_HOME/bin" >> ~/.bash_profile
    source ~/.bash_profile
else
    sed -i '/^export JAVA_HOME=.*/c\export JAVA_HOME=\/usr\/lib\/jvm\/java-1.8.0-openjdk' ~/.bash_profile
fi

# Use RVM to install the desired Ruby version, then install the gems.
$HOOT_HOME/scripts/ruby/rvm-install.sh
$HOOT_HOME/scripts/ruby/gem-install.sh

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

# Install Google Chrome and ChromeDriver.
$HOOT_HOME/scripts/chrome/chrome-install.sh
$HOOT_HOME/scripts/chrome/driver-install.sh

if [ ! -f bin/osmosis ]; then
    echo "### Installing Osmosis"
    mkdir -p ~/bin
    if [ ! -f osmosis-latest.tgz ]; then
      wget --quiet http://bretth.dev.openstreetmap.org/osmosis-build/osmosis-latest.tgz
    fi
    mkdir -p ~/bin/osmosis_src
    tar -zxf osmosis-latest.tgz -C ~/bin/osmosis_src
    ln -s ~/bin/osmosis_src/bin/osmosis ~/bin/osmosis
fi

# Need to figure out a way to do this automagically
#PG_VERSION=$(sudo -u postgres psql -c 'SHOW SERVER_VERSION;' | egrep -o '[0-9]{1,}\.[0-9]{1,}')
#PG_VERSION=9.5
PG_VERSION=$(psql --version | egrep -o '[0-9]{1,}\.[0-9]{1,}')

if ! grep --quiet "psql-" ~/.bash_profile; then
    echo "Adding PostGres path vars to profile..."
    echo "export PATH=\$PATH:/usr/pgsql-$PG_VERSION/bin" >> ~/.bash_profile
    source ~/.bash_profile
fi

if [ ! -f /etc/ld.so.conf.d/postgres$PG_VERSION.conf ]; then
    sudo sh -c "echo '/usr/pgsql-$PG_VERSION/lib' > /etc/ld.so.conf.d/postgres$PG_VERSION.conf"
    sudo ldconfig
fi

# Make sure that we are in the home directory when downloading packages
cd ~

# Tweak the FGDB version so we can get the filename
FGDB_VERSION2=`echo $FGDB_VERSION | sed 's/\./_/g;'`

if ! $( hash ogrinfo >/dev/null 2>&1 && ogrinfo --version | grep -q $GDAL_VERSION && ogrinfo --formats | grep -q FileGDB ); then
    if [ ! -f gdal-$GDAL_VERSION.tar.gz ]; then
        echo "### Downloading GDAL $GDAL_VERSION source..."
        wget --quiet http://download.osgeo.org/gdal/$GDAL_VERSION/gdal-$GDAL_VERSION.tar.gz
    fi
    if [ ! -d gdal-$GDAL_VERSION ]; then
        echo "### Extracting GDAL $GDAL_VERSION source..."
        tar zxfp gdal-$GDAL_VERSION.tar.gz
    fi

    if [ ! -f FileGDB_API_${FGDB_VERSION2}-64.tar.gz ]; then
        echo "### Downloading FileGDB API source..."
        wget --quiet https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_${FGDB_VERSION}/FileGDB_API_${FGDB_VERSION2}-64.tar.gz
    fi

    if [ ! -d /usr/local/FileGDB_API ]; then
        echo "### Extracting FileGDB API source & installing lib..."
        sudo mkdir -p /usr/local/FileGDB_API && sudo tar xfp FileGDB_API_${FGDB_VERSION2}-64.tar.gz --directory /usr/local/FileGDB_API --strip-components 1
        sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
    fi

    echo "### Building GDAL $GDAL_VERSION w/ FileGDB..."
    export PATH=/usr/local/lib:/usr/local/bin:$PATH
    cd gdal-$GDAL_VERSION
    touch config.rpath
    echo "GDAL: configure"
    sudo ./configure --quiet --with-fgdb=/usr/local/FileGDB_API --with-pg=/usr/pgsql-$PG_VERSION/bin/pg_config --with-python CFLAGS='-std=c11' CXXFLAGS='-std=c++11'
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

    # Update the GDAL_DATA folder in ~/.bash_profile
    if ! grep --quiet GDAL_DATA ~/.bash_profile; then
      echo "Adding GDAL data path to profile..."
      echo "export GDAL_DATA=`gdal-config --datadir`" >> ~/.bash_profile
      source ~/.bash_profile
    fi
fi

if ! mocha --version &>/dev/null; then
    echo "### Installing mocha for plugins test..."
    sudo npm install --silent -g mocha@3.5.3
    # Clean up after the npm install
    sudo rm -rf ~/tmp
fi


echo "### Configuring Postgres..."
cd /tmp # Stop postgres "could not change directory to" warnings

# NOTE: These have been changed to pg9.5
# Postgresql startup
#PGSETUP_INITDB_OPTIONS="-E 'UTF-8' --lc-collate='en_US.UTF-8' --lc-ctype='en_US.UTF-8'"


sudo PGSETUP_INITDB_OPTIONS="-E 'UTF-8' --lc-collate='en_US.UTF-8' --lc-ctype='en_US.UTF-8'" /usr/pgsql-$PG_VERSION/bin/postgresql95-setup initdb
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

echo "Restarting postgres"
sudo systemctl restart postgresql-$PG_VERSION

# Install Hadoop.
$HOOT_HOME/scripts/hadoop/hadoop-install.sh

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
$HOOT_HOME/scripts/tomcat/tomcat8/centos7/tomcat8_install.sh

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

##### These two are next to do.
echo "### Installing node-mapnik-server..."
sudo cp $HOOT_HOME/node-mapnik-server/systemd/node-mapnik.service /etc/systemd/system/node-mapnik.service
sudo sed -i "s/SERVICE_USER/$VMUSER/g" /etc/systemd/system/node-mapnik.service
sudo sed -i "s|HOOT_HOME|$HOOT_HOME|g" /etc/systemd/system/node-mapnik.service
# Make sure all npm modules are installed
cd $HOOT_HOME/node-mapnik-server
npm install --silent
# Clean up after the npm install
rm -rf ~/tmp

echo "### Installing node-export-server..."
sudo cp $HOOT_HOME/node-export-server/systemd/node-export.service /etc/systemd/system/node-export.service
sudo sed -i "s/SERVICE_USER/$VMUSER/g" /etc/systemd/system/node-export.service
sudo sed -i "s|HOOT_HOME|$HOOT_HOME|g" /etc/systemd/system/node-export.service
# Make sure all npm modules are installed
cd $HOOT_HOME/node-export-server
npm install --silent
# Clean up after the npm install
rm -rf ~/tmp

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
