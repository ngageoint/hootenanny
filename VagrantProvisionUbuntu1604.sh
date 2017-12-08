#!/usr/bin/env bash
set -e
set -x

###################################################
# VERY IMPORTANT: Set the $HOOT_HOME environment  #
# variable prior to running this script if ~/hoot #
# isn't the correct location for HOOT_HOME        #
###################################################
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME="~/hoot"
fi
echo HOOT_HOME: $HOOT_HOME
#################################################

# Common set of file versions
source $HOOT_HOME/VagrantProvisionVars.sh

#
# Initial basic provisioning script for Ubuntu1604
#

VMUSER=`id -u -n`
echo USER: $VMUSER
VMGROUP=`groups | grep -o $VMUSER`
echo GROUP: $VMGROUP

cd ~
source ~/.profile

#To get rid of "dpkg-preconfigure: unable to re-open stdin: No such file or directory" warnings
export DEBIAN_FRONTEND=noninteractive
if [ -f /etc/apt/apt.conf.d/70debconf ]; then
  sudo sed -i 's/ --apt//' /etc/apt/apt.conf.d/70debconf
fi

echo "Updating OS..."
sudo apt-get -qq update > Ubuntu_upgrade.txt 2>&1
sudo apt-get -q -y upgrade >> Ubuntu_upgrade.txt 2>&1
sudo apt-get -q -y dist-upgrade >> Ubuntu_upgrade.txt 2>&1

echo "### Setup NTP..."
sudo apt-get -q -y install ntp
sudo service ntp stop
sudo ntpd -gq
sudo service ntp start

if ! java -version 2>&1 | grep --quiet $JDK_VERSION; then
    echo "### Installing Java 8..."

    echo "$JDK_MD5  $JDK_TAR" > ./jdk.md5

    if [ ! -f ./$JDK_TAR ] || ! md5sum -c ./jdk.md5; then
        echo "Downloading ${JDK_TAR}...."
        sudo wget --quiet --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" $JDK_URL
        echo "Finished download of ${JDK_TAR}"
    fi

    sudo mkdir -p /usr/lib/jvm
    sudo rm -rf /usr/lib/jvm/oracle_jdk8

    sudo tar -xzf ./$JDK_TAR
    sudo chown -R root:root ./jdk$JDK_VERSION
    sudo mv -f ./jdk$JDK_VERSION /usr/lib/jvm/oracle_jdk8

    sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/oracle_jdk8/jre/bin/java 9999
    sudo update-alternatives --install /usr/bin/javac javac /usr/lib/jvm/oracle_jdk8/bin/javac 9999
    echo "### Done with Java 8 install..."
fi

if [ ! -f /etc/apt/sources.list.d/pgdg.list ]; then
    echo "### Adding PostgreSQL repository to apt..."
    sudo bash -c "echo 'deb http://apt.postgresql.org/pub/repos/apt/ '$(lsb_release -cs)'-pgdg main' > /etc/apt/sources.list.d/pgdg.list"
    wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
    sudo apt-get -qq -y update >> Ubuntu_upgrade.txt 2>&1
    sudo apt-get -q -y upgrade >> Ubuntu_upgrade.txt 2>&1
fi

# Install an ancient version of NodeJS
echo "### Installing the PPA for an ancient version of NodeJS"
curl -sL https://deb.nodesource.com/setup_0.10 | sudo -E bash -

echo "### Installing an ancient version of NodeJS"
sudo apt-get -q -y --allow-downgrades install nodejs=0.10.48-1nodesource1~xenial1


echo "### Installing dependencies from repos..."
sudo apt-get -q -y install \
 asciidoc \
 automake \
 ccache \
 curl \
 dblatex \
 docbook-xml \
 doxygen \
 g++ \
 gdb \
 git \
 git-core \
 gnuplot \
 graphviz \
 htop \
 lcov \
 libboost-all-dev \
 libboost-dev \
 libcppunit-dev \
 libcv-dev \
 libgeos++-dev \
 libglpk-dev \
 libicu-dev \
 liblog4cxx10-dev \
 libnewmat10-dev \
 libogdi3.2-dev \
 libopencv-dev \
 libpq-dev \
 libproj-dev \
 libprotobuf-dev \
 libqt4-dev \
 libqt4-sql-psql \
 libqt4-sql-sqlite \
 libqtwebkit-dev \
 libstxxl-dev \
 libv8-dev \
 maven \
 node-gyp \
 openssh-server \
 patch \
 pgadmin3 \
 postgresql-9.5 \
 postgresql-client-9.5 \
 postgresql-contrib-9.5 \
 postgresql-9.5-postgis-2.3 \
 postgresql-9.5-postgis-scripts \
 postgresql-client-9.5 \
 protobuf-compiler \
 python \
 python-dev \
 python-matplotlib \
 python-pip \
 python-setuptools \
 ruby \
 ruby-dev \
 source-highlight \
 swig \
 texinfo \
 unzip \
 w3m \
 x11vnc \
 xsltproc \
 xvfb \
 zlib1g-dev \
 > Ubuntu_install.txt 2>&1

# Need to check these:
#  libjson-spirit-dev \
#  libxerces-c28 \
#  texlive-lang-arabic \
#  texlive-lang-cyrillic \
#  texlive-lang-hebrew \
#  nodejs-dev \
#  nodejs-legacy \


if ! dpkg -l | grep --quiet dictionaries-common; then
    # See /usr/share/doc/dictionaries-common/README.problems for details
    # http://www.linuxquestions.org/questions/debian-26/dpkg-error-processing-dictionaries-common-4175451951/
    sudo apt-get -q -y install dictionaries-common >> Ubuntu_upgrade.txt 2>&1

    sudo /usr/share/debconf/fix_db.pl

    sudo apt-get -q -y install wamerican-insane >> Ubuntu_upgrade.txt 2>&1

    sudo /usr/share/debconf/fix_db.pl
    sudo dpkg-reconfigure -f noninteractive dictionaries-common
fi

sudo apt-get -y autoremove

echo "### Configuring environment..."

if ! grep --quiet "export HOOT_HOME" ~/.profile; then
    echo "Adding hoot home to profile..."
    echo "export HOOT_HOME=$HOOT_HOME" >> ~/.profile
    echo "export PATH=\$PATH:\$HOOT_HOME/bin" >> ~/.profile
    source ~/.profile
fi

if ! grep --quiet "export JAVA_HOME" ~/.profile; then
    echo "Adding Java home to profile..."
    echo "export JAVA_HOME=/usr/lib/jvm/oracle_jdk8" >> ~/.profile
    source ~/.profile
else
    sed -i '/^export JAVA_HOME=.*/c\export JAVA_HOME=\/usr\/lib\/jvm\/oracle_jdk8' ~/.profile
fi

if ! grep --quiet "PATH=" ~/.profile; then
    echo "Adding path vars to profile..."
    echo "export PATH=\$PATH:\$JAVA_HOME/bin:~/bin:$HOOT_HOME/bin" >> ~/.profile
    source ~/.profile
fi

# Use RVM to install the desired Ruby version, then install the gems.
$HOOT_HOME/scripts/ruby/rvm-install.sh
$HOOT_HOME/scripts/ruby/gem-install.sh

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

# Install Google Chrome and ChromeDriver.
$HOOT_HOME/scripts/chrome/chrome-install.sh
$HOOT_HOME/scripts/chrome/driver-install.sh

sudo apt-get autoremove -y

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

if ! $( hash ogrinfo >/dev/null 2>&1 && ogrinfo --formats | grep --quiet FileGDB ); then
    if [ ! -f gdal-${GDAL_VERSION}.tar.gz ]; then
        echo "### Downloading GDAL $GDAL_VERSION source..."
        wget --quiet http://download.osgeo.org/gdal/$GDAL_VERSION/gdal-${GDAL_VERSION}.tar.gz
    fi
    if [ ! -d gdal-${GDAL_VERSION} ]; then
        echo "### Extracting GDAL $GDAL_VERSION source..."
        tar zxfp gdal-${GDAL_VERSION}.tar.gz
    fi

    FGDB_VERSION2=`echo $FGDB_VERSION | sed 's/\./_/g;'`

    # FGDB 1.5 is required to compile using g++ >= 5.1
    # https://trac.osgeo.org/gdal/wiki/FileGDB#HowtodealwithGCC5.1C11ABIonLinux
    if [ ! -f FileGDB_API_${FGDB_VERSION2}-64gcc51.tar.gz ]; then
        echo "### Downloading FileGDB API source..."
        wget --quiet $FGDB_URL/FileGDB_API_${FGDB_VERSION2}-64gcc51.tar.gz
    fi

    if [ ! -d /usr/local/FileGDB_API/lib ]; then
        echo "### Extracting FileGDB API source & installing lib..."
        sudo mkdir -p /usr/local/FileGDB_API && sudo tar xfp FileGDB_API_${FGDB_VERSION2}-64gcc51.tar.gz --directory /usr/local/FileGDB_API --strip-components 1
        sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
    fi

    echo "### Building GDAL $GDAL_VERSION w/ FileGDB..."
    export PATH=/usr/local/lib:/usr/local/bin:$PATH
    cd gdal-${GDAL_VERSION}
    touch config.rpath
    echo "GDAL: configure"
    ./configure --quiet --with-static-proj4 --with-fgdb=/usr/local/FileGDB_API --with-pg=/usr/bin/pg_config --with-python
    echo "GDAL: make"
    make -sj$(nproc) > GDAL_Build.txt 2>&1
    echo "GDAL: install"
    sudo make -s install >> GDAL_Build.txt 2>&1
    cd swig/python
    echo "GDAL: python build"
    python setup.py build >> GDAL_Build.txt 2>&1
    echo "GDAL: python install"
    sudo python setup.py install >> GDAL_Build.txt 2>&1
    sudo ldconfig
    cd ~

    # Update the GDAL_DATA folder in ~/.profile
    if ! grep --quiet GDAL_DATA ~/.profile; then
      echo "Adding GDAL data path to profile..."
      echo "export GDAL_DATA=`gdal-config --datadir`" >> ~/.profile
      source ~/.profile
    fi
fi

if ! mocha --version &>/dev/null; then
    echo "### Installing mocha for plugins test..."
    sudo npm install --silent -g mocha
    # Clean up after the npm install
    sudo rm -rf ~/tmp/*
fi

sudo apt -y -qq remove postgresql-9.6
sudo apt -y -qq autoremove

# If the DB encoding isn't correcct, delete and recreate the DB.
if ! sudo -u postgres psql $DB_NAME -c "SHOW SERVER_ENCODING" | grep UTF8; then
  echo "Now stop the postgresql instance so we can recreate the DB w/ UTF8 encoding."
  sudo service postgresql stop

  sudo rm -rf /var/lib/postgresql/9.5/main/ && sudo -u postgres /usr/lib/postgresql/9.5/bin/initdb -E UTF8 -D /var/lib/postgresql/9.5/main/

  echo "Starting postgresql back up."
  sudo service postgresql start
fi

# NOTE: These have been changed to pg9.5
if ! sudo -u postgres psql -lqt | grep -i --quiet hoot; then
    echo "### Creating Services Database..."
    sudo -u postgres createuser --superuser hoot
    sudo -u postgres psql -c "alter user hoot with password 'hoottest';"
    sudo -u postgres createdb hoot --owner=hoot
    sudo -u postgres createdb wfsstoredb --owner=hoot
    sudo -u postgres psql -d hoot -c 'create extension hstore;'
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'" > /dev/null
    sudo -u postgres psql -d wfsstoredb -c 'create extension postgis;' > /dev/null
fi

if ! grep -i --quiet HOOT /etc/postgresql/9.5/main/postgresql.conf; then
echo "### Tuning PostgreSQL..."
sudo -u postgres sed -i.bak s/^max_connections/\#max_connections/ /etc/postgresql/9.5/main/postgresql.conf
sudo -u postgres sed -i.bak s/^shared_buffers/\#shared_buffers/ /etc/postgresql/9.5/main/postgresql.conf
sudo -u postgres bash -c "cat >> /etc/postgresql/9.5/main/postgresql.conf" <<EOT

#--------------
# Hoot Settings
#--------------
max_connections = 1000
shared_buffers = 1024MB
max_files_per_process = 1000
work_mem = 16MB
maintenance_work_mem = 256MB
autovacuum = off
EOT
fi

# Update shared memory limits in OS
if ! sysctl -e kernel.shmmax | grep --quiet 1173741824; then
    echo "### Setting kernel.shmmax..."
    sudo sysctl -w kernel.shmmax=1173741824
    sudo sh -c "echo 'kernel.shmmax=1173741824' >> /etc/sysctl.conf"
fi
if ! sysctl -e kernel.shmall | grep --quiet 2097152; then
    echo "### Setting kernel.shmall..."
    sudo sysctl -w kernel.shmall=2097152
    sudo sh -c "echo 'kernel.shmall=2097152' >> /etc/sysctl.conf"
fi

sudo service postgresql restart

cd $HOOT_HOME
source ./SetupEnv.sh

if [ ! "$(ls -A hoot-ui)" ]; then
    echo "hoot-ui is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

if dpkg -l | grep --quiet tomcat6; then
    echo "Disabling Tomcat 6 service"

    # Shut down tomcat6 service
    sudo service tomcat6 stop

    # Deregister tomcat6 service from autostart
    sudo update-rc.d -f tomcat6 remove

    #uninstall Tomcat
    sudo apt-get -y purge tomcat6
    sudo apt-get -y autoremove
    sudo rm -f /etc/default/tomcat6*
    sudo rm -rf /etc/tomcat6
    sudo rm -rf /usr/share/tomcat6
    sudo sed -i '/^export TOMCAT6_HOME/d' ~/.profile
fi

TOMCAT_HOME=/usr/share/tomcat8

# Install Tomcat 8
$HOOT_HOME/scripts/tomcat/tomcat8/ubuntu/tomcat8_install.sh

# Configure Tomcat
if ! grep --quiet TOMCAT8_HOME ~/.profile; then
    echo "### Adding Tomcat to profile..."
    echo "export TOMCAT8_HOME=$TOMCAT_HOME" >> ~/.profile
    source ~/.profile
fi

if [ -f $HOOT_HOME/conf/LocalHoot.json ]; then
    echo "Removing LocalHoot.json..."
    rm -f $HOOT_HOME/conf/LocalHoot.json
fi

if [ -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf ]; then
    echo "Removing services local.conf..."
    rm -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf
fi

cd ~

#
# NOTE: Hadoop install has been removed
#

echo "### Installing node-mapnik-server..."
sudo cp $HOOT_HOME/node-mapnik-server/init.d/node-mapnik-server /etc/init.d
sudo chmod a+x /etc/init.d/node-mapnik-server
# Make sure all npm modules are installed
cd $HOOT_HOME/node-mapnik-server
npm install --silent
# Clean up after the npm install
rm -rf ~/tmp/*

echo "### Installing node-export-server..."
sudo cp $HOOT_HOME/node-export-server/init.d/node-export-server /etc/init.d
sudo chmod a+x /etc/init.d/node-export-server
# Make sure all npm modules are installed
cd $HOOT_HOME/node-export-server
npm install --silent
# Clean up after the npm install
rm -rf ~/tmp/*

cd $HOOT_HOME

rm -rf $HOOT_HOME/ingest
mkdir -p $HOOT_HOME/ingest/processed

rm -rf $HOOT_HOME/upload
mkdir -p $HOOT_HOME/upload

# Update marker file date now that dependency and config stuff has run
# The make command will exit and provide a warning to run 'vagrant provision'
# if the marker file is older than this file (VagrantProvision.sh)
touch Vagrant.marker
# Now we are ready to build Hoot.  The VagrantBuild.sh script will build Hoot.

# switch to auto mode and use the highest priority installed alternatives for Java.
sudo update-alternatives --auto java
sudo update-alternatives --auto javac
