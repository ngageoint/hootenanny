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
# https://hoot-support.s3.amazonaws.com/american-english-insane.bz2
# https://hoot-support.s3.amazonaws.com/words1.sqlite.bz2
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

VMUSER=`id -u -n`
echo USER: $VMUSER
VMGROUP=`groups | grep -o $VMUSER`
echo GROUP: $VMGROUP

HOOT_HOME=~/hoot
echo HOOT_HOME: $HOOT_HOME
cd ~
source ~/.bash_profile

# Keep VagrantBuild.sh happy
#ln -s ~/.bash_profile ~/.profile

# add EPEL repo for extra packages
echo "### Add epel repo ###" > CentOS_upgrade.txt
sudo yum -y install epel-release >> CentOS_upgrade.txt 2>&1

# add the Postgres repo
echo "### Add Postgres repo ###" > CentOS_upgrade.txt
sudo rpm -Uvh http://yum.postgresql.org/9.5/redhat/rhel-7-x86_64/pgdg-centos95-9.5-3.noarch.rpm >> CentOS_upgrade.txt 2>&1

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


# Install Java8
# Make sure that we are in ~ before trying to wget & install stuff
cd ~

echo "### Installing the repo for an ancient version of NodeJS"
curl --silent --location https://rpm.nodesource.com/setup | sudo bash -

echo "### Installing an ancient version of NodeJS"
sudo yum install -y \
  nodejs-0.10.46 \
  nodejs-devel-0.10.46


# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
sudo yum -y install \
    asciidoc \
    autoconf \
    automake \
    boost-devel \
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
    if [ ! -f jdk-8u144-linux-x64.rpm ]; then
      JDKURL=http://download.oracle.com/otn-pub/java/jdk/8u144-b01/090f390dda5b47b9b721c7dfaa008135/jdk-8u144-linux-x64.rpm
      wget --quiet --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" $JDKURL
    fi
    sudo yum -y install ./jdk-8u144-linux-x64.rpm
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

# Stxxl:
if [ -d /home/centos/hoot-deps/stxxl ]
    cd /home/centos/hoot-deps/stxxl
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
    if hash qmake-qt5 >/dev/null 2>&1 ; then
      sudo alternatives --install /usr/bin/qmake qmake /usr/bin/qmake-qt5 500
    else
      echo "##### No qmake! #####"
    fi
fi

# We need this big dictionary for text matching. On Ubuntu, this is a package
if [ ! -f /usr/share/dict/american-english-insane ]; then
    echo "### Installing american-english-insane dictionary..."
    if [ -f /home/centos/hoot-deps/american-english-insane.bz2 ]
        sudo bash -c "bzcat /home/centos/hoot-deps/american-english-insane.bz2 > /usr/share/dict/american-english-insane"
    else
        wget --quiet -N https://hoot-support.s3.amazonaws.com/american-english-insane.bz2
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
    echo "export HOOT_HOME=~/hoot" >> ~/.bash_profile
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

# if ! grep --quiet "export HADOOP_HOME" ~/.bash_profile; then
#     echo "Adding Hadoop home to profile..."
#     echo "export HADOOP_HOME=~/hadoop" >> ~/.bash_profile
#     echo "export PATH=\$PATH:\$HADOOP_HOME/bin" >> ~/.bash_profile
#     source ~/.bash_profile
# fi

# if ! ruby -v | grep --quiet 2.3.0; then
#     # Ruby via rvm - from rvm.io
#     # Running this twice so that it should not error out
#     gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3 || \
#         gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3  2>&1
#
#     curl -sSL https://raw.githubusercontent.com/rvm/rvm/master/binscripts/rvm-installer | bash -s stable
#
# #     source /home/$VMUSER/.rvm/scripts/rvm
#     source ~/.rvm/scripts/rvm
#
#     stdbuf -o L -e L rvm install ruby-2.3
#     rvm --default use 2.3

# Don't install documentation for gems
# cat > ~/.gemrc <<EOT
#   install: --no-document
#   update: --no-document
# EOT
# fi

# gem installs are *very* slow, hence all the checks in place here to facilitate debugging
# gem list --local | grep -q mime-types
# if [ $? -eq 1 ]; then
#    #sudo gem install mime-types -v 2.6.2
#    gem install mime-types
# fi
# gem list --local | grep -q cucumber
# if [ $? -eq 1 ]; then
#    #sudo gem install cucumber
#    gem install cucumber
# fi
# gem list --local | grep -q capybara-webkit
# if [ $? -eq 1 ]; then
#    #sudo gem install capybara-webkit
#    gem install capybara-webkit
# fi
# gem list --local | grep -q selenium-webdriver
# if [ $? -eq 1 ]; then
#    #sudo gem install selenium-webdriver
#    gem install selenium-webdriver
# fi
# gem list --local | grep -q rspec
# if [ $? -eq 1 ]; then
#    #sudo gem install rspec
#    gem install rspec
# fi
# gem list --local | grep -q capybara-screenshot
# if [ $? -eq 1 ]; then
#    #sudo gem install capybara-screenshot
#    gem install capybara-screenshot
# fi
# gem list --local | grep -q selenium-cucumber
# if [ $? -eq 1 ]; then
#    #sudo gem install selenium-cucumber
#    gem install selenium-cucumber
# fi

# Make sure that we are in ~ before trying to wget & install stuff
cd ~


# if  ! rpm -qa | grep google-chrome-stable; then
#     echo "### Installing Chrome..."
#     if [ ! -f google-chrome-stable_current_x86_64.rpm ]; then
#       wget --quiet https://dl.google.com/linux/direct/google-chrome-stable_current_x86_64.rpm
#     fi
#     sudo yum -y install ./google-chrome-stable_current_*.rpm
# fi
#
# if [ ! -f bin/chromedriver ]; then
#     echo "### Installing Chromedriver..."
#     mkdir -p ~/bin
#     if [ ! -f chromedriver_linux64.zip ]; then
# #       LATEST_RELEASE="`wget --quiet -O- http://chromedriver.storage.googleapis.com/LATEST_RELEASE`"
# #       wget --quiet http://chromedriver.storage.googleapis.com/$LATEST_RELEASE/chromedriver_linux64.zip
#
# # Errors with the latest release (2/31) wanting glibc v2.18 when only glibc v2.17 is available
# # https://bugs.chromium.org/p/chromedriver/issues/detail?id=1894#c2
#       wget --quiet http://chromedriver.storage.googleapis.com/2.30/chromedriver_linux64.zip
#     fi
#     unzip -d ~/bin chromedriver_linux64.zip
# else
#   LATEST_RELEASE="`wget --quiet -O- http://chromedriver.storage.googleapis.com/LATEST_RELEASE`"
#   if [[ "$(chromedriver --version)" != "ChromeDriver $LATEST_RELEASE."* ]]; then
#     echo "### Updating Chromedriver"
#     rm ~/bin/chromedriver
#     rm ~/chromedriver_linux64.zip
#     wget --quiet http://chromedriver.storage.googleapis.com/$LATEST_RELEASE/chromedriver_linux64.zip
#     unzip -o -d ~/bin chromedriver_linux64.zip
#   fi
# fi

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
#PG_VERSION=$(sudo -u postgres psql -c 'SHOW SERVER_VERSION;' | egrep -o '[0-9]{1,}\.[0-9]{1,}'); do
PG_VERSION=9.5

if ! grep --quiet "psql-" ~/.bash_profile; then
    echo "Adding PostGres path vars to profile..."
    echo "export PATH=\$PATH:/usr/pgsql-$PG_VERSION/bin" >> ~/.bash_profile
    source ~/.bash_profile
fi

if [ ! -f /etc/ld.so.conf.d/postgres$PG_VERSION.conf ]; then
    sudo sh -c "echo '/usr/pgsql-$PG_VERSION/lib' > /etc/ld.so.conf.d/postgres$PG_VERSION.conf"
    sudo ldconfig
fi

# For convenience, set the version of GDAL and FileGDB to download and install
GDAL_VERSION=2.1.4
FGDB_VERSION=1.5.1
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
fi

if ! mocha --version &>/dev/null; then
    echo "### Installing mocha for translations test..."
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
    sudo -u postgres psql -d $DB_NAME -c 'create extension hstore;'
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

##### Hadoop #####
cd ~
# hoot has only been tested successfully with hadoop 0.20.2, which is not available from public repos,
# so purposefully not installing hoot from the repos.
# if ! hash hadoop >/dev/null 2>&1 ; then
#   echo "Installing Hadoop..."
#   if [ ! -f hadoop-0.20.2.tar.gz ]; then
#     wget --quiet https://archive.apache.org/dist/hadoop/core/hadoop-0.20.2/hadoop-0.20.2.tar.gz
#   fi
#
#   if [ ! -f ~/.ssh/id_rsa ]; then
#     ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
#     cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
#     ssh-keyscan -H localhost >> ~/.ssh/known_hosts
#   fi
#   chmod 600 ~/.ssh/authorized_keys
#
#   #cd /usr/local
#   cd ~
#   sudo tar -zxf ~/hadoop-0.20.2.tar.gz
#   sudo chown -R $VMUSER:$VMGROUP hadoop-0.20.2
#   sudo ln -s hadoop-0.20.2 hadoop
#   sudo chown -R $VMUSER:$VMGROUP hadoop
#   cd hadoop
#   sudo find . -type d -exec chmod a+rwx {} \;
#   sudo find . -type f -exec chmod a+rw {} \;
#   cd ~
#
# sudo rm -f $HADOOP_HOME/conf/core-site.xml
# sudo bash -c "cat >> $HADOOP_HOME/conf/core-site.xml" <<EOT
#
# <configuration>
#   <property>
#     <name>fs.default.name</name>
#     <value>hdfs://localhost:9000/</value>
#   </property>
# </configuration>
# EOT
# sudo rm -f $HADOOP_HOME/conf/mapred-site.xml
# sudo bash -c "cat >> $HADOOP_HOME/conf/mapred-site.xml" <<EOT
#
# <configuration>
#   <property>
#     <name>mapred.job.tracker</name>
#     <value>localhost:9001</value>
#   </property>
#   <property>
#     <name>mapred.job.tracker.http.address</name>
#     <value>0.0.0.0:50030</value>
#   </property>
#   <property>
#     <name>mapred.task.tracker.http.address</name>
#     <value>0.0.0.0:50060</value>
#   </property>
#   <property>
#     <name>mapred.child.java.opts</name>
#     <value>-Xmx2048m</value>
#   </property>
#   <property>
#     <name>mapred.map.tasks</name>
#     <value>17</value>
#   </property>
#   <property>
#     <name>mapred.tasktracker.map.tasks.maximum</name>
#     <value>4</value>
#   </property>
#   <property>
#     <name>mapred.tasktracker.reduce.tasks.maximum</name>
#     <value>2</value>
#   </property>
#   <property>
#     <name>mapred.reduce.tasks</name>
#     <value>1</value>
#   </property>
# </configuration>
# EOT
# sudo rm -f $HADOOP_HOME/conf/hdfs-site.xml
# sudo bash -c "cat >> $HADOOP_HOME/conf/hdfs-site.xml" <<EOT
#
# <configuration>
#   <property>
#     <name>dfs.secondary.http.address</name>
#     <value>0.0.0.0:50090</value>
#   </property>
#   <property>
#     <name>dfs.datanode.address</name>
#     <value>0.0.0.0:50010</value>
#   </property>
#   <property>
#     <name>dfs.datanode.http.address</name>
#     <value>0.0.0.0:50075</value>
#   </property>
#   <property>
#     <name>dfs.datanode.ipc.address</name>
#     <value>0.0.0.0:50020</value>
#   </property>
#   <property>
#     <name>dfs.http.address</name>
#     <value>0.0.0.0:50070</value>
#   </property>
#   <property>
#     <name>dfs.datanode.https.address</name>
#     <value>0.0.0.0:50475</value>
#   </property>
#   <property>
#     <name>dfs.https.address</name>
#     <value>0.0.0.0:50470</value>
#   </property>
#   <property>
#     <name>dfs.replication</name>
#     <value>2</value>
#   </property>
#   <property>
#     <name>dfs.umaskmode</name>
#     <value>002</value>
#   </property>
#   <property>
#     <name>fs.checkpoint.dir</name>
#     <value>$HADOOP_HOME/dfs/namesecondary</value>
#   </property>
#   <property>
#     <name>dfs.name.dir</name>
#     <value>$HADOOP_HOME/dfs/name</value>
#   </property>
#   <property>
#     <name>dfs.data.dir</name>
#     <value>$HADOOP_HOME/dfs/data</value>
#   </property>
# </configuration>
# EOT
#
#   sudo sed -i.bak 's/# export JAVA_HOME=\/usr\/lib\/j2sdk1.5-sun/export JAVA_HOME=\/usr\/java\/jdk1.8.0_144/g' $HADOOP_HOME/conf/hadoop-env.sh
#   sudo sed -i.bak 's/#include <pthread.h>/#include <pthread.h>\n#include <unistd.h>/g' $HADOOP_HOME/src/c++/pipes/impl/HadoopPipes.cc
#
#   sudo mkdir -p $HADOOP_HOME/dfs/name/current
#   # this could perhaps be more strict
#   sudo chmod -R 777 $HADOOP_HOME
#   sudo chmod go-w $HADOOP_HOME/bin $HADOOP_HOME
#   echo 'Y' | hadoop namenode -format
#
#   cd /lib
#   sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
#   cd /lib64
#   sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
#   cd ~
#
#   # test hadoop out
#   #stop-all.sh
#   #start-all.sh
#   #hadoop fs -ls /
#   #hadoop jar ./hadoop-0.20.2-examples.jar pi 2 100
# fi
##### End Hadoop #####


# Get ready to build Hoot
cd $HOOT_HOME
source ./SetupEnv.sh

# Not sure if we really need this...
if [ ! "$(ls -A hoot-ui)" ]; then
    echo "hoot-ui is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

# echo "### Installing Tomcat8..."
# TOMCAT_HOME=/usr/share/tomcat8

# Install Tomcat 8
# $HOOT_HOME/scripts/tomcat/tomcat8/centos7/tomcat8_install.sh

# Configure Tomcat for the user
# if ! grep --quiet TOMCAT8_HOME ~/.bash_profile; then
#     echo "### Adding Tomcat to profile..."
#     echo "export TOMCAT8_HOME=$TOMCAT_HOME" >> ~/.bash_profile
#     source ~/.bash_profile
# fi

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

# echo "### Installing node-export-server..."
# sudo cp $HOOT_HOME/node-export-server/systemd/node-export.service /etc/systemd/system/node-export.service
# # Make sure all npm modules are installed
# cd $HOOT_HOME/node-export-server
# npm install --silent
# # Clean up after the npm install
# rm -rf ~/tmp

cd $HOOT_HOME

# # Update marker file date now that dependency and config stuff has run
# # The make command will exit and provide a warning to run 'vagrant provision'
# # if the marker file is older than this file (VagrantProvision.sh)
# touch Vagrant.marker
#
# # Setup and clean out the directories that the UI uses
# if [ ! -d "$HOOT_HOME/userfiles/ingest/processed" ]; then
#     mkdir -p $HOOT_HOME/userfiles/ingest/processed
# fi
#
# # wipe out all dirs. tmp and upload now reside under $HOOT_HOME/userfiles/
# rm -rf $HOOT_HOME/upload
# rm -rf $HOOT_HOME/tmp
#
# if [ -d "$HOOT_HOME/data/reports" ]; then
#     echo "Moving contents of $HOOT_HOME/data/reports to $HOOT_HOME/userfiles/"
#     cp -R $HOOT_HOME/data/reports $HOOT_HOME/userfiles/
#     rm -rf $HOOT_HOME/data/reports
# fi
#
# if [ -d "$HOOT_HOME/customscript" ]; then
#     echo "Moving contents of $HOOT_HOME/customscript to $HOOT_HOME/userfiles/"
#     cp -R $HOOT_HOME/customscript $HOOT_HOME/userfiles/
#     rm -rf $HOOT_HOME/customscript
# fi
#
# if [ -d "$HOOT_HOME/ingest" ]; then
#     echo "Moving contents of $HOOT_HOME/ingest to $HOOT_HOME/userfiles/"
#     cp -R $HOOT_HOME/ingest $HOOT_HOME/userfiles/
#     rm -rf $HOOT_HOME/ingest
# fi
#
# # Always start with a clean $HOOT_HOME/userfiles/tmp
# rm -rf $HOOT_HOME/userfiles/tmp
#
# # This is defensive!
# # We do this so that Tomcat doesnt. If it does, it screws the permissions up
# mkdir -p $HOOT_HOME/userfiles/tmp
#
# # OK, this is seriously UGLY but it fixes an NFS problem
# chmod -R 777 $HOOT_HOME/userfiles
#
# # This is very ugly.
# # If we don't have access to the directory where HOOT_HOME is, Tomcat chokes
# chmod go+rx ~


# Now build Hoot
source ./SetupEnv.sh

echo "### Configuring Hoot..."
echo HOOT_HOME: $HOOT_HOME

# Going to remove this so that it gets updated
if [ -f missing ]; then
  rm -f missing
fi

aclocal && autoconf && autoheader && automake --add-missing --copy && ./configure --with-rnd --with-services --with-josm

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
