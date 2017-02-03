#!/usr/bin/env bash

#TODO: Fix HOOT_HOME
HOOT_HOME=$HOME/hoot
echo HOOT_HOME: $HOOT_HOME
cd ~
source ~/.bash_profile

# Keep VagrantBuild.sh happy
#ln -s ~/.bash_profile ~/.profile

# Find out what user we have: vagrant or ubuntu
# This comes from a user difference in one of the Vagrant boxes
if [ "$(getent passwd vagrant)" ]; then
  VMUSER=vagrant
else
  if [ "$(getent passwd ubuntu)" ]; then
    VMUSER=ubuntu
  fi
fi


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
if  ! rpm -qa | grep jdk-8u111-linux; then
    echo "### Installing Java8..."
    if [ ! -f jdk-8u111-linux-x64.rpm ]; then
      JDKURL=http://download.oracle.com/otn-pub/java/jdk/8u111-b14/jdk-8u111-linux-x64.rpm
      wget --quiet --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" $JDKURL
    fi
    sudo yum -y install ./jdk-8u111-linux-x64.rpm
fi

echo "### Installing the repo for an ancient version of NodeJS"
curl --silent --location https://rpm.nodesource.com/setup | sudo bash -

echo "### Installing an ancient version of NodeJS"
sudo yum install -y \
  nodejs-0.10.46 \
  nodejs-devel-0.10.46

# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
sudo yum -y install \
    automake \
    autoconf \
    bc \
    boost-devel \
    ccache \
    gcc \
    gcc-c++ \
    cppunit-devel \
    gdb \
    git \
    git-core \
    geos-devel \
    libtool \
    m4 \
    qt \
    qt-common \
    qt-devel \
    qtwebkit \
    qtwebkit-devel \
    postgis2_95 \
    postgresql95 \
    postgresql95-contrib \
    postgresql95-devel \
    postgresql95-server \
    proj \
    proj-devel \
    stxxl \
    stxxl-devel \
    python  \
    python-devel \
    python-matplotlib \
    python-pip  \
    python-setuptools \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    protobuf \
    protobuf-compiler \
    protobuf-devel \
    libicu-devel \
    maven \
    glpk \
    glpk-devel \
    unzip \
    v8 \
    v8-devel \
    words \
    tex* \
    w3m \
    doxygen \



#     asciidoc \
#     texlive \
#     dblatex \
#     texlive-cyrillic \
cd ~

echo "##### Temp installs #####"
# sudo yum -y install \
#     gdal \
#     gdal-devel \
#     gdal-python \

# Fix missing qmake
if ! hash qmake >/dev/null 2>&1 ; then
    if hash qmake-qt4 >/dev/null 2>&1 ; then
      sudo alternatives --install /usr/bin/qmake qmake /usr/bin/qmake-qt4 500
    else
      echo "##### No qmake! #####"
    fi
fi


if ! ruby -v | grep --quiet 2.3.0; then
    # Ruby via rvm - from rvm.io
    gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3 2>&1

    curl -sSL https://raw.githubusercontent.com/rvm/rvm/master/binscripts/rvm-installer | bash -s stable

#     source /home/$VMUSER/.rvm/scripts/rvm
    source ~/.rvm/scripts/rvm

    stdbuf -o L -e L rvm install ruby-2.3
    rvm --default use 2.3

# Don't install documentation for gems
cat > ~/.gemrc <<EOT
  install: --no-document
  update: --no-document
EOT
fi

# gem installs are *very* slow, hence all the checks in place here to facilitate debugging
gem list --local | grep -q mime-types
if [ $? -eq 1 ]; then
   #sudo gem install mime-types -v 2.6.2
   gem install mime-types
fi
gem list --local | grep -q cucumber
if [ $? -eq 1 ]; then
   #sudo gem install cucumber
   gem install cucumber
fi
gem list --local | grep -q capybara-webkit
if [ $? -eq 1 ]; then
   #sudo gem install capybara-webkit
   gem install capybara-webkit
fi
gem list --local | grep -q selenium-webdriver
if [ $? -eq 1 ]; then
   #sudo gem install selenium-webdriver
   gem install selenium-webdriver
fi
gem list --local | grep -q rspec
if [ $? -eq 1 ]; then
   #sudo gem install rspec
   gem install rspec
fi
gem list --local | grep -q capybara-screenshot
if [ $? -eq 1 ]; then
   #sudo gem install capybara-screenshot
   gem install capybara-screenshot
fi
gem list --local | grep -q selenium-cucumber
if [ $? -eq 1 ]; then
   #sudo gem install selenium-cucumber
   gem install selenium-cucumber
fi

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

# This is commented out since it tries to install qt3. Once we get the core building, this will get
# re-added
# if  ! rpm -qa | grep google-chrome-stable; then
#     echo "### Installing Chrome..."
#     if [ ! -f google-chrome-stable_current_x86_64.rpm ]; then
#       wget --quiet https://dl.google.com/linux/direct/google-chrome-stable_current_x86_64.rpm
#     fi
#     sudo yum -y install ./google-chrome-stable_current_*.rpm
# fi

if [ ! -f bin/chromedriver ]; then
    echo "### Installing Chromedriver..."
    mkdir -p $HOME/bin
    if [ ! -f chromedriver_linux64.zip ]; then
      LATEST_RELEASE="`wget --quiet -O- http://chromedriver.storage.googleapis.com/LATEST_RELEASE`"
      wget --quiet http://chromedriver.storage.googleapis.com/$LATEST_RELEASE/chromedriver_linux64.zip
    fi
    unzip -d $HOME/bin chromedriver_linux64.zip
else
  LATEST_RELEASE="`wget --quiet -O- http://chromedriver.storage.googleapis.com/LATEST_RELEASE`"
  if [[ "$(chromedriver --version)" != "ChromeDriver $LATEST_RELEASE."* ]]; then
    echo "### Updating Chromedriver"
    rm $HOME/bin/chromedriver
    rm $HOME/chromedriver_linux64.zip
    wget --quiet http://chromedriver.storage.googleapis.com/$LATEST_RELEASE/chromedriver_linux64.zip
    unzip -o -d $HOME/bin chromedriver_linux64.zip
  fi
fi

if [ ! -f bin/osmosis ]; then
    echo "### Installing Osmosis"
    mkdir -p $HOME/bin
    if [ ! -f osmosis-latest.tgz ]; then
      wget --quiet http://bretth.dev.openstreetmap.org/osmosis-build/osmosis-latest.tgz
    fi
    mkdir -p $HOME/bin/osmosis_src
    tar -zxf osmosis-latest.tgz -C $HOME/bin/osmosis_src
    ln -s $HOME/bin/osmosis_src/bin/osmosis $HOME/bin/osmosis
fi

echo "### Configureing Postgres..."
# Need to figure out a way to do this automagically
#PG_VERSION=$(sudo -u postgres psql -c 'SHOW SERVER_VERSION;' | egrep -o '[0-9]{1,}\.[0-9]{1,}'); do
PG_VERSION=9.5

cd /tmp # Stop postgres "could not change directory to" warnings

# Postgresql startup
sudo /usr/pgsql-$PG_VERSION/bin/postgresql95-setup initdb
sudo systemctl start postgresql-$PG_VERSION
sudo systemctl enable postgresql-$PG_VERSION

if ! sudo -u postgres psql -lqt | grep -i --quiet hoot; then
    echo "### Creating Services Database..."
    sudo -u postgres createuser --superuser hoot
    sudo -u postgres psql -c "alter user hoot with password 'hoottest';"
    sudo -u postgres createdb hoot --owner=hoot
    sudo -u postgres createdb wfsstoredb --owner=hoot
    sudo -u postgres psql -d hoot -c 'create extension hstore;'
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'" > /dev/null
    sudo -u postgres psql -d wfsstoredb -c 'create extension postgis;' > /dev/null
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geometry_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geography_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on spatial_ref_sys TO PUBLIC;"
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

#  if [ "! ogrinfo --formats | grep --quiet FileGDB" ]; then
#     if [ ! -f gdal-1.10.1.tar.gz ]; then
#         echo "### Downloading GDAL source..."
#         wget --quiet http://download.osgeo.org/gdal/1.10.1/gdal-1.10.1.tar.gz
#     fi
#     if [ ! -d gdal-1.10.1 ]; then
#         echo "### Extracting GDAL source..."
#         tar zxfp gdal-1.10.1.tar.gz
#     fi
#
#     if [ ! -f FileGDB_API_1_4-64.tar.gz ]; then
#         echo "### Downloading FileGDB API source..."
#         wget --quiet https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_1_4-64.tar.gz
#     fi
#     if [ ! -d /usr/local/FileGDB_API ]; then
#         echo "### Extracting FileGDB API source & installing lib..."
#         sudo mkdir -p /usr/local/FileGDB_API && sudo tar xfp FileGDB_API_1_4-64.tar.gz --directory /usr/local/FileGDB_API --strip-components 1
#         sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
#     fi
#
#     # compile gdal
#     echo "### Building GDAL w/ FileGDB..."
#     export PATH=/usr/local/lib:/usr/local/bin:$PATH
#     cd gdal-1.10.1
#     echo "GDAL: configure"
#     sudo ./configure --quiet --with-fgdb=/usr/local/FileGDB_API --with-pg=/usr/pgsql-$PG_VERSION/bin/pg_config --with-python
#     echo "GDAL: make"
#     sudo make -sj$(nproc) > GDAL_Build.txt 2>&1
#     echo "GDAL: install"
#     sudo make -s install >> GDAL_Build.txt 2>&1
#     cd swig/python
#     echo "GDAL: python build"
#     python setup.py build >> GDAL_Build.txt 2>&1
#     echo "GDAL: python install"
#     sudo python setup.py install >> GDAL_Build.txt 2>&1
#     sudo ldconfig
#     cd ~
#     GDAL_DATA=/usr/local/share/gdal
#     GDAL_LIB_DIR=/usr/local/lib
#
#     # Remove gdal libs installed by libgdal-dev that interfere with
#     # renderdb-export-server using gdal libs compiled from source (fgdb support)
#     if [ -f "/usr/lib/libgdal.*" ]; then
#         echo "Removing GDAL libs installed by libgdal-dev..."
#         sudo rm /lib64/libgdal.*
#     fi
# fi # End GDAL

if ! hash ogrinfo >/dev/null 2>&1 || ogrinfo --formats | grep --quiet FileGDB; then
    if [ ! -f gdal-1.10.1.tar.gz ]; then
        echo "### Downloading GDAL source..."
        wget --quiet http://download.osgeo.org/gdal/1.10.1/gdal-1.10.1.tar.gz
    fi
    if [ ! -d gdal-1.10.1 ]; then
        echo "### Extracting GDAL source..."
        tar zxfp gdal-1.10.1.tar.gz
    fi

    if [ ! -f FileGDB_API_1_4-64.tar.gz ]; then
        echo "### Downloading FileGDB API source..."
        wget --quiet https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_1_4-64.tar.gz
    fi
    if [ ! -d /usr/local/FileGDB_API ]; then
        echo "### Extracting FileGDB API source & installing lib..."
        sudo mkdir -p /usr/local/FileGDB_API && sudo tar xfp FileGDB_API_1_4-64.tar.gz --directory /usr/local/FileGDB_API --strip-components 1
        sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
    fi

    echo "### Building GDAL w/ FileGDB..."
    export PATH=/usr/local/lib:/usr/local/bin:$PATH
    cd gdal-1.10.1
    touch config.rpath
    echo "GDAL: configure"
    sudo ./configure --quiet --with-fgdb=/usr/local/FileGDB_API --with-pg=/usr/pgsql-$PG_VERSION/bin/pg_config --with-python
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
    sudo rm -rf $HOME/tmp
fi

# # Configure Java
# if ! grep --quiet "export JAVA_HOME" ~/.bash_profile; then
#     echo "Adding Java home to profile..."
#     export JAVA_HOME=/usr/java/jdk1.8.0_111 >> ~/.bash_profile
#     source ~/.bash_profile
# fi

# Configure qmake
# if ! grep --quiet "export QMAKE" ~/.bash_profile; then
#     echo "### Adding qmake to profile..."
#     echo "export QMAKE=/usr/lib64/qt4/bin/qmake" >> ~/.bash_profile
#     echo "export PATH=\$PATH:/usr/lib64/qt4/bin" >> ~/.bash_profile
#     echo "export QTDIR=/usr/lib64/qt4/bin" >> ~/.bash_profile
#     source ~/.bash_profile
# fi

cd $HOOT_HOME
source ./SetupEnv.sh

if [ ! "$(ls -A hoot-ui)" ]; then
    echo "hoot-ui is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

# if dpkg -l | grep --quiet tomcat6; then
#     echo "Disabling Tomcat 6 service"
#
#     # Shut down tomcat6 service
#     sudo service tomcat6 stop
#
#     # Deregister tomcat6 service from autostart
#     sudo update-rc.d -f tomcat6 remove
#
#     #uninstall Tomcat
#     sudo apt-get -y purge tomcat6
#     sudo apt-get -y autoremove
#     sudo rm -f /etc/default/tomcat6*
#     sudo rm -rf /etc/tomcat6
#     sudo rm -rf /usr/share/tomcat6
#     sudo sed -i '/^export TOMCAT6_HOME/d' ~/.profile
# fi

echo "### Installing Tomcat8..."
# NOTE: We could pull the RPM from the Hoot repo and install it instead of doing all of the manual steps.
#sudo bash -c "cat >> /etc/yum.repos.d/hoot.repo" <<EOT
# [hoot]
# name=hoot
# baseurl=https://s3.amazonaws.com/hoot-rpms/snapshot/el6/
# gpgcheck=0
#EOT

# Or
# wget https://s3.amazonaws.com/hoot-rpms/snapshot/el6/tomcat8-8.5.8-1.noarch.rpm
# rpm -ivh tomcat8-8.5.8-1.noarch.rpm


# Manual Install
sudo groupadd tomcat8
sudo useradd -M -s /bin/nologin -g tomcat8 -d /var/lib/tomcat8 tomcat8

if [ ! -f apache-tomcat-8.5.9.tar.gz ]; then
    wget http://apache.mirrors.ionfish.org/tomcat/tomcat-8/v8.5.9/bin/apache-tomcat-8.5.9.tar.gz
fi

sudo mkdir /var/lib/tomcat8
sudo tar xvf apache-tomcat-8*tar.gz -C /var/lib/tomcat8 --strip-components=1
cd /var/lib/tomcat8
sudo chgrp -R tomcat8 /var/lib/tomcat8
sudo chmod -R g+r conf
sudo chmod g+x conf
sudo chown -R tomcat8 webapps/ work/ temp/ logs/

sudo bash -c "cat >> /etc/systemd/system/tomcat8.service" <<EOT
# Systemd unit file for tomcat8
[Unit]
Description=Apache Tomcat Web Application Container
After=syslog.target network.target

[Service]
Type=forking

Environment=JAVA_HOME=/usr/java/jdk1.8.0_111
Environment=CATALINA_PID=/var/lib/tomcat8/temp/tomcat8.pid
Environment=CATALINA_HOME=/var/lib/tomcat8
Environment=CATALINA_BASE=/var/lib/tomcat8
Environment='CATALINA_OPTS=-Xms512M -Xmx2048M -server -XX:+UseParallelGC'
Environment='JAVA_OPTS=-Djava.awt.headless=true -Djava.security.egd=file:/dev/./urandom'

ExecStart=/var/lib/tomcat8/bin/startup.sh
ExecStop=/bin/kill -15 $MAINPID

User=tomcat8
Group=tomcat8
UMask=0007
RestartSec=10
Restart=always

[Install]
WantedBy=multi-user.targetEOT
EOT

# Start Tomcat8
sudo systemctl daemon-reload
sudo systemctl start tomcat8
sudo systemctl enable tomcat8


# Configure Tomcat for the user
if ! grep --quiet TOMCAT8_HOME ~/.bash_profile; then
    echo "### Adding Tomcat to profile..."
    echo "export TOMCAT8_HOME=/var/lib/tomcat8" >> ~/.bash_profile
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

cd ~

echo "### Installing node-mapnik-server..."
sudo cp $HOOT_HOME/node-mapnik-server/init.d/node-mapnik-server /etc/init.d
sudo chmod a+x /etc/init.d/node-mapnik-server
# Make sure all npm modules are installed
cd $HOOT_HOME/node-mapnik-server
npm install --silent
# Clean up after the npm install
rm -rf $HOME/tmp

echo "### Installing node-export-server..."
sudo cp $HOOT_HOME/node-export-server/init.d/node-export-server /etc/init.d
sudo chmod a+x /etc/init.d/node-export-server
# Make sure all npm modules are installed
cd $HOOT_HOME/node-export-server
npm install --silent
# Clean up after the npm install
rm -rf $HOME/tmp

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

exit

####################################################
# From RPM Job
    apache-maven \
      CharLS-devel \
      ImageMagick \
      ant \
      apr-devel \
      apr-util-devel \
      armadillo-devel \
      bison \
      cairo-devel \
      cfitsio-devel \
      chrpath \
      cppunit-devel \
      createrepo \
      ctags \
      curl-devel \
      doxygen \
      emacs \
      emacs \
      emacs-el \
      erlang \
      erlang \
      expat-devel \
      flex \
      fontconfig-devel \
      freexl-devel \
      g2clib-static \
      gd-devel \
      geos-devel \
      giflib-devel \
      giflib-devel \
      graphviz \
      hdf-devel \
      hdf-static \
      hdf5-devel \
      help2man \
      info \
      libX11-devel \
      libXrandr-devel \
      libXrender-devel \
      libXt-devel \
      libdap-devel \
      libdrm-devel \
      libgta-devel \
      libicu-devel \
      libjpeg-turbo-devel \
      libotf \
      libpng-devel \
      librx-devel \
      libspatialite-devel \
      libtool \
      libxslt \
      libxslt \
      lua-devel \
      m17n-lib* \
      m4 \
      mysql-devel \
      netcdf-devel \
      numpy \
      pango-devel \
      pcre-devel \
      php-devel \
      poppler-devel \
      proj-devel \
      pygtk2 \
      python-argparse \
      python-devel \
      python-devel \
      readline-devel \
      rpm-build \
      rpm-build \
      ruby \
      ruby-devel \
      sqlite-devel \
      swig \
      tetex-tex4ht \
      tex* \
      transfig \
      unixODBC-devel \
      w3m \
      wget \
      words \
      xerces-c-devel \
      xz-devel \
      zlib-devel \

# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
sudo yum -y install \
 asciidoc  \
 automake  \
 boost-devel  \
 ccache  \
 cppunit-devel  \
 curl  \
 dblatex \
 doxygen  \
 fontconfig-devel  \
 freetype-devel  \
 gcc-c++  \
 gdal-devel \
 gdb  \
 geos-devel  \
 gettext  \
 git  \
 git-core  \
 glpk-devel  \
 gnuplot  \
 graphviz  \
 htop  \
 java  \
 lcov  \
 libX11-devel \
 libpng-devel \
 libtool  \
 libxslt  \
 liquibase  \
 log4cxx-devel \
 maven  \
 nodejs-devel \
 npm  \
 ogdi-devel  \
 opencv-devel  \
 openssh-server  \
 patch  \
 pgadmin3  \
 poppler  \
 postgis2_95 \
 postgresql95 \
 postgresql95-contrib \
 postgresql95-devel \
 postgresql95-server \
 proj-devel  \
 protobuf-compiler  \
 protobuf-devel  \
 python  \
 python-devel \
 python-matplotlib \
 python-pip  \
 python-setuptools \
 qt-devel  \
 ruby  \
 ruby-devel  \
 source-highlight  \
 swig  \
 texinfo-tex  \
 texlive-arabxetex  \
 texlive-collection-langcyrillic  \
 unzip  \
 w3m  \
 wget  \
 words  \
 x11vnc  \
 xerces-c  \
 xorg-x11-server-Xvfb  \
 zlib-devel  \
 >> CentOS_upgrade.txt 2>&1


# Orig List
#  asciidoc  \
#  automake  \
#  boost-devel  \
#  ccache  \
#  cppunit-devel  \
#  curl  \
#  dblatex \
#  doxygen  \
#  fontconfig-devel  \
#  freetype-devel  \
#  gcc-c++  \
#  gdal-devel \
#  gdb  \
#  gettext  \
#  git  \
#  git-core  \
#  glpk-devel  \
#  gnuplot  \
#  graphviz  \
#  htop  \
#  java-1.8.0-openjdk  \
#  java-1.8.0-openjdk-debug  \
#  lcov  \
#  libX11-devel \
#  libicu-devel  \
#  libpng  \
#  libpng-devel \
#  libtool  \
#  libxslt  \
#  liquibase  \
#  log4cxx-devel \
#  maven  \
#  nodejs-devel \
#  npm  \
#  ogdi-devel  \
#  opencv-devel  \
#  openssh-server  \
#  patch  \
#  pgadmin3  \
#  poppler  \
#  postgis  \
#  postgresql \
#  postgresql-contrib \
#  postgresql-devel  \
#  postgresql-libs  \
#  postgresql-server  \
#  proj-devel  \
#  protobuf-compiler  \
#  protobuf-devel  \
#  python  \
#  python-devel \
#  python-matplotlib \
#  python-pip  \
#  python-setuptools \
#  qt-devel  \
#  qt5-qtwebkit-devel  \
#  ruby  \
#  ruby-devel  \
#  source-highlight  \
#  swig  \
#  texinfo-tex  \
#  texlive-arabxetex  \
#  texlive-collection-langcyrillic  \
#  tomcat  \
#  unzip  \
#  w3m  \
#  wget  \
#  words  \
#  x11vnc  \
#  xerces-c  \
#  xorg-x11-server-Xvfb  \
#  zlib-devel  \

# TODO: Investigate these packages from the Ubuntu14.04 provisioning
#libcv
#newmat
#libproj-dev
#libqt4-sql-psql
#libjson-spirit-dev
#libstxll-dev
#nodejs-legacy
#geos-devel
#libboost-all-dev
#texlive-lang-hebrew
#libqt4-sql-sqlite
#postgresql-client-9.5
#postgresql-9.5-postgis-scripts
#postgresql-client-9.5
#gdal-devel \
#stxxl-devel \

# TODO: Investigate the necessity of these
#texlive-arabxetex \
#texlive-collection-langcyrillic \





