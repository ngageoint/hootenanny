#!/usr/bin/env bash

#TODO: Fix HOOT_HOME
HOOT_HOME=$HOME/hoot
echo HOOT_HOME: $HOOT_HOME
cd ~
source ~/.bash_profile

echo "Updating OS..."
sudo yum -qq update > CentOS_upgrade.txt 2>&1
sudo yum -q -y upgrade >> CentOS_upgrade.txt 2>&1

echo "### Setup NTP..."
sudo yum -q -y install ntp
#TODO: Better way to do this?
sudo systemctl stop ntpd 
sudo ntpd -gq
sudo systemctl start ntpd

# add EPEL repo for extra packages
sudo yum -y install epel-release >> CentOS_upgrade.txt 2>&1

# TODO: Install and configure postgresql 9.5, add a check to see if this repo is already installed
# install postgresql rpm
#sudo rpm -ivh http://yum.postgresql.org/9.5/redhat/rhel-7-x86_64/pgdg-centos95-9.5-2.noarch.rpm

# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
sudo yum -y install asciidoc automake boost-devel ccache cppunit-devel curl dblatex doxygen >> CentOS_upgrade.txt 2>&1
sudo yum -y install fontconfig-devel freetype-devel gcc-c++ gdb gdal-devel gettext git git-core >> CentOS_upgrade.txt 2>&1
sudo yum -y install glpk-devel gnuplot graphviz htop java-1.8.0-openjdk java-1.8.0-openjdk-debug >> CentOS_upgrade.txt 2>&1
sudo yum -y install lcov libicu-devel libpng libpng-devel libtool libxslt libX11-devel liquibase >> CentOS_upgrade.txt 2>&1
sudo yum -y install log4cxx-devel maven nodejs-devel npm ogdi-devel opencv-devel openssh-server >> CentOS_upgrade.txt 2>&1
sudo yum -y install patch pgadmin3 poppler postgis postgresql postgresql-contrib postgresql-devel >> CentOS_upgrade.txt 2>&1
sudo yum -y install postgresql-libs postgresql-server proj-devel protobuf-compiler protobuf-devel >> CentOS_upgrade.txt 2>&1
sudo yum -y install python python-devel python-matplotlib python-pip python-setuptools qt-devel >> CentOS_upgrade.txt 2>&1
sudo yum -y install qt5-qtwebkit-devel ruby ruby-devel source-highlight swig texinfo-tex texlive-arabxetex >> CentOS_upgrade.txt 2>&1
sudo yum -y install texlive-collection-langcyrillic tomcat unzip vim wget words w3m xerces-c >> CentOS_upgrade.txt 2>&1
sudo yum -y install xorg-x11-server-Xvfb x11vnc zlib-devel >> CentOS_upgrade.txt 2>&1

# TODO: Install and configure postgresql 9.5
#postgis2_95 \
#postgresql95
#postgresql95-devel \
#postgresql95-server \

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

# Configure Java
if ! grep --quiet "export JAVA_HOME" ~/.bash_profile; then
    echo "Adding Java home to profile..."
    echo "export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.101-3.b13.el7_2.x86_64/jre" >> ~/.bash_profile
    source ~/.bash_profile
fi

# Configure qmake
if ! grep --quiet "export QMAKE" ~/.bash_profile; then
    echo "### Adding qmake to profile..."
    echo "export QMAKE=/usr/lib64/qt5/bin/qmake" >> ~/.bash_profile
    source ~/.bash_profile
fi

# Ruby via rvm - from rvm.io
gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
curl -sSL https://raw.githubusercontent.com/rvm/rvm/master/binscripts/rvm-installer | bash -s stable
source /home/vagrant/.rvm/scripts/rvm
rvm install ruby-2.3
rvm --default use 2.3

# Don't install documentation for gems
cat > ~/.gemrc <<EOT
  install: --no-document
  update: --no-document
EOT

# gem installs are *very* slow, hence all the checks in place here to facilitate debugging
echo "### Installing cucumber gems..."
gem list --local | grep -q mime-types
if [ $? -eq 1 ]; then
   gem install mime-types
fi
gem list --local | grep -q cucumber
if [ $? -eq 1 ]; then
   gem install cucumber
fi
gem list --local | grep -q capybara-webkit
if [ $? -eq 1 ]; then
   gem install capybara-webkit
fi
gem list --local | grep -q selenium-webdriver
if [ $? -eq 1 ]; then
   gem install selenium-webdriver
fi
gem list --local | grep -q rspec
if [ $? -eq 1 ]; then
   gem install rspec
fi
gem list --local | grep -q capybara-screenshot
if [ $? -eq 1 ]; then
   gem install capybara-screenshot
fi
gem list --local | grep -q selenium-cucumber
if [ $? -eq 1 ]; then
   gem install selenium-cucumber
fi

# Make sure that we are in ~ before trying to wget & install stuff
cd ~
if  ! rpm -qa | grep google-chrome-stable; then
    echo "### Installing Chrome..."
    if [ ! -f google-chrome-stable_current_x86_64.rpm ]; then
      wget --quiet https://dl.google.com/linux/direct/google-chrome-stable_current_x86_64.rpm
    fi
    sudo yum -y install ./google-chrome-stable_current_*.rpm
fi

if [ ! -f bin/chromedriver ]; then
    echo "### Installing Chromedriver..."
    mkdir -p $HOME/bin
    if [ ! -f chromedriver_linux64.zip ]; then
      wget --quiet http://chromedriver.storage.googleapis.com/2.14/chromedriver_linux64.zip
    fi
    unzip -d $HOME/bin chromedriver_linux64.zip
fi

# Build and install GEOS
if [ ! -f geos-3.4.2.tar.bz2 ]; then
    echo "### Downloading GEOS source..."
    wget --quiet http://download.osgeo.org/geos/geos-3.4.2.tar.bz2
fi
if [ ! -d geos-3.4.2 ]; then
    echo "#### Extracting GEOS source..."
    tar -xvf geos-3.4.2.tar.bz2 
fi
# TODO: This check doesn't seem to work as intended
if ! geos --version &> /dev/null; then
    cd geos-3.4.2
    echo "### Building geos..."
    echo "GEOS: configure" 
    sudo ./configure --enable-python
    echo "GEOS: make"
    sudo make -sj$(nproc) > GEOS_Build.txt 2>&1
    echo "GEOS: install"
    sudo make -s install >> GEOS_Build.txt 2>&1
    cd ~
fi

# TODO: Add check for previously installed GDAL
# download gdal for compiling, we do this so we get the desired version and can configure it 
# if ! ogrinfo --formats | grep --quiet FileGDB; then
    if [ ! -f gdal-1.10.1.tar.gz ]; then
        echo "### Downloading GDAL source..."
        wget --quiet http://download.osgeo.org/gdal/1.10.1/gdal-1.10.1.tar.gz
    fi
    if [ ! -d gdal-1.10.1 ]; then
        echo "### Extracting GDAL source..."
        tar zxfp gdal-1.10.1.tar.gz
    fi
    if [ ! -f FileGDB_API_1_3-64.tar.gz ]; then
        echo "### Downloading FileGDB API source..."
        wget --quiet http://downloads2.esri.com/Software/FileGDB_API_1_3-64.tar.gz
    fi
    if [ ! -d /usr/local/FileGDB_API ]; then
        echo "### Extracting FileGDB API source & installing lib..."
        sudo tar xfp FileGDB_API_1_3-64.tar.gz --directory /usr/local
        sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
    fi

    # compile gdal
    echo "### Building GDAL w/ FileGDB..."
    export PATH=/usr/local/lib:/usr/local/bin:$PATH
    cd gdal-1.10.1
    echo "GDAL: configure"
    sudo ./configure --quiet --with-fgdb=/usr/local/FileGDB_API --with-pg=/usr/bin/pg_config --with-python
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
#fi

if ! mocha --version &>/dev/null; then
    echo "### Installing mocha for plugins test..."
    sudo npm install --silent -g mocha
    # Clean up after the npm install
    sudo rm -rf $HOME/tmp
fi

# initialize and start postgresql
sudo postgresql-setup initdb
sudo service postgresql start
sudo systemctl enable postgresql


# TODO: Initialize and start postgresql?
#/usr/pgsql-9.5/bin/postgresql95-setup initdb
#chkconfig postgresql-9.5 on
#systemctl enable postgresql-9.5.service

# TODO: Configure postgresql for hoot
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

#if ! grep -i --quiet HOOT /etc/postgresql/9.5/main/postgresql.conf; then
#echo "### Tuning PostgreSQL..."
#sudo -u postgres sed -i.bak s/^max_connections/\#max_connections/ /etc/postgresql/9.5/main/postgresql.conf
#sudo -u postgres sed -i.bak s/^shared_buffers/\#shared_buffers/ /etc/postgresql/9.5/main/postgresql.conf
#sudo -u postgres bash -c "cat >> /etc/postgresql/9.5/main/postgresql.conf" <<EOT

#--------------
# Hoot Settings
#--------------
#max_connections = 1000
#shared_buffers = 1024MB
#max_files_per_process = 1000
#work_mem = 16MB
#maintenance_work_mem = 256MB
#autovacuum = off
#EOT
#fi

#sudo service postgresql restart

# Configure Tomcat
# TODO: Investigate TOMCAT6
if ! grep --quiet TOMCAT6_HOME ~/.bash_profile; then
    echo "### Adding Tomcat to profile..."
    echo "export TOMCAT6_HOME=/var/lib/tomcat" >> ~/.bash_profile
    source ~/.bash_profile
fi

GDAL_DATA=/usr/local/share/gdal
GDAL_LIB_DIR=/usr/local/lib

# Remove gdal libs installed by libgdal-dev that interfere with
# renderdb-export-server using gdal libs compiled from source (fgdb support)
if [ -f "/usr/lib/libgdal.*" ]; then
    echo "Removing GDAL libs installed by libgdal-dev..."
    sudo rm /usr/lib/libgdal.*
fi

