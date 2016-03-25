#!/usr/bin/env bash

HOOT_HOME=$HOME/hoot
echo HOOT_HOME: $HOOT_HOME
cd ~
source ~/.profile

#To get rid of "dpkg-preconfigure: unable to re-open stdin: No such file or directory" warnings
export DEBIAN_FRONTEND=noninteractive

echo "Updating OS..."
sudo apt-get -qq update > Ubuntu_upgrade.txt 2>&1
sudo apt-get -q -y upgrade >> Ubuntu_upgrade.txt 2>&1
sudo apt-get -q -y dist-upgrade >> Ubuntu_upgrade.txt 2>&1
sudo apt-get -q -y install ntp

if [ ! -f /etc/apt/sources.list.d/pgdg.list ]; then
    echo "### Adding PostgreSQL repository to apt..."
    sudo bash -c "echo 'deb http://apt.postgresql.org/pub/repos/apt/ '$(lsb_release -cs)'-pgdg main' > /etc/apt/sources.list.d/pgdg.list"
    wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
    sudo apt-get -qq -y update >> Ubuntu_upgrade.txt 2>&1
    sudo apt-get -q -y upgrade >> Ubuntu_upgrade.txt 2>&1
fi

echo "### Installing dependencies from repos..."
sudo apt-get -q -y install texinfo g++ libicu-dev libqt4-dev git-core libboost-dev libcppunit-dev libcv-dev libopencv-dev libgdal-dev liblog4cxx10-dev libnewmat10-dev libproj-dev python-dev libjson-spirit-dev automake1.11 protobuf-compiler libprotobuf-dev gdb libqt4-sql-psql libgeos++-dev swig lcov tomcat6 openjdk-7-jdk openjdk-7-dbg maven libstxxl-dev nodejs-dev nodejs-legacy doxygen xsltproc asciidoc pgadmin3 curl npm libxerces-c28 libglpk-dev libboost-all-dev source-highlight texlive-lang-arabic texlive-lang-hebrew texlive-lang-cyrillic graphviz w3m python-setuptools python python-pip git ccache libogdi3.2-dev gnuplot python-matplotlib libqt4-sql-sqlite ruby ruby-dev xvfb zlib1g-dev patch x11vnc unzip >> Ubuntu_upgrade.txt 2>&1

if ! dpkg -l | grep --quiet dictionaries-common; then
    # See /usr/share/doc/dictionaries-common/README.problems for details
    # http://www.linuxquestions.org/questions/debian-26/dpkg-error-processing-dictionaries-common-4175451951/
    sudo apt-get -q -y install dictionaries-common

    sudo /usr/share/debconf/fix_db.pl

    sudo apt-get -q -y install wamerican-insane

    sudo /usr/share/debconf/fix_db.pl
    sudo dpkg-reconfigure dictionaries-common
fi

sudo apt-get -y autoremove

echo "### Installing node-mapnik-server..."
sudo cp $HOOT_HOME/node-mapnik-server/init.d/node-mapnik-server /etc/init.d
sudo chmod a+x /etc/init.d/node-mapnik-server
# Make sure all npm modules are installed
cd $HOOT_HOME/node-mapnik-server
sudo npm install --quiet
cd ~

# checkpoint for snapshotting - see snapshotting notes in Developer's Guide
#exit 0

echo "### Configuring environment..."

if ! grep --quiet "export JAVA_HOME" ~/.profile; then
    echo "Adding Java home to profile..."
    echo "export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64" >> ~/.profile
    source ~/.profile
fi

if ! grep --quiet "PATH=" ~/.profile; then
    echo "Adding path vars to profile..."
    echo "export PATH=\$PATH:\$HOME/.gem/ruby/1.9.1/bin:\$HOME/bin:$HOOT_HOME/bin" >> ~/.profile
    source ~/.profile
fi

# gem installs are *very* slow, hence all the checks in place here to facilitate debugging
echo "### Installing cucumber gems..."
gem list --local | grep -q mime-types
if [ $? -eq 1 ]; then
   sudo gem install mime-types -v 2.6.2
fi
gem list --local | grep -q capybara
if [ $? -eq 1 ]; then
   sudo gem install capybara -v 2.5.0
fi
gem list --local | grep -q cucumber
if [ $? -eq 1 ]; then
   sudo gem install cucumber
fi
gem list --local | grep -q capybara-webkit
if [ $? -eq 1 ]; then
   sudo gem install capybara-webkit
fi
gem list --local | grep -q selenium-webdriver
if [ $? -eq 1 ]; then
   sudo gem install selenium-webdriver
fi
gem list --local | grep -q rspec
if [ $? -eq 1 ]; then
   sudo gem install rspec
fi
gem list --local | grep -q capybara-screenshot
if [ $? -eq 1 ]; then
   sudo gem install capybara-screenshot
fi
gem list --local | grep -q selenium-cucumber
if [ $? -eq 1 ]; then
   sudo gem install selenium-cucumber
fi    

# Make sure that we are in ~ before trying to wget & install stuff 
cd ~

if  ! dpkg -l | grep google-chrome-stable; then
    echo "### Installing Chrome..."
    if [ ! -f google-chrome-stable_current_amd64.deb ]; then
      wget --quiet https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
    fi
    sudo dpkg -i google-chrome-stable_current_amd64.deb
    sudo apt-get -f -y -q install
fi

if [ ! -f bin/chromedriver ]; then
    echo "### Installing Chromedriver..."
    mkdir -p $HOME/bin
    if [ ! -f chromedriver_linux64.zip ]; then
      wget --quiet http://chromedriver.storage.googleapis.com/2.14/chromedriver_linux64.zip
    fi
    unzip -d $HOME/bin chromedriver_linux64.zip
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

# Hoot Baseline is PostgreSQL 9.1 and PostGIS 1.5, so we need a deb file and
# then remove 9.5
if  ! dpkg -l | grep  postgresql-9.1-postgis-[0-9]; then
    echo "### Installing PostgreSQL 9.1..."
    if [ ! -f postgresql-9.1-postgis_1.5.3-2_amd64.deb ]; then
      wget --quiet http://launchpadlibrarian.net/86690107/postgresql-9.1-postgis_1.5.3-2_amd64.deb
    fi
    sudo dpkg -i postgresql-9.1-postgis_1.5.3-2_amd64.deb
    sudo apt-get -f -q -y install
    # fixes missing dependency of postgis 1.5 by installing postgresql 9.1. 9.1 is installed listening on the default port, 5432. It unfortunately also installs postgres 9.5 but we remove that cleanly in the following steps, while leaving postgres 9.1 untouched
    echo "### Removing PostgreSQL 9.5..."
    sudo apt-get purge -y postgresql-9.5 postgresql-client-9.5 postgresql-9.5-postgis-scripts
    sudo apt-get -q -y install postgresql-contrib-9.1
fi

if ! ogrinfo --formats | grep --quiet FileGDB; then
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
fi

if ! grep --quiet NODE_PATH ~/.profile; then
    echo "### Installing node js dependencies..."
    sudo npm config set registry http://registry.npmjs.org/
    sudo npm install --silent -g xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler
    echo 'Adding NODE_PATH to user environment...'
    echo 'export NODE_PATH=/usr/local/lib/node_modules' >> ~/.profile
    source ~/.profile
fi

# Module needed for OSM API db test
if [ ! -d $HOME/.cpan ]; then
    echo "### Installing Perl XML::Simple module..."
    (echo y;echo o conf prerequisites_policy follow;echo o conf commit)|sudo cpan
    sudo perl -MCPAN -e 'install XML::Simple' > /dev/null
fi

if ! sudo -u postgres psql -lqt | grep -i --quiet hoot; then
    echo "### Creating Services Database..."
    sudo -u postgres createuser --superuser hoot
    sudo -u postgres psql -c "alter user hoot with password 'hoottest';"
    sudo -u postgres createdb hoot --owner=hoot
    sudo -u postgres createdb wfsstoredb --owner=hoot
    sudo -u postgres psql -d hoot -c 'create extension hstore;'
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'" > /dev/null
    sudo -u postgres psql -d wfsstoredb -f /usr/share/postgresql/9.1/contrib/postgis-1.5/postgis.sql > /dev/null
    sudo -u postgres psql -d wfsstoredb -f /usr/share/postgresql/9.1/contrib/postgis-1.5/spatial_ref_sys.sql > /dev/null
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geometry_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geography_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on spatial_ref_sys TO PUBLIC;"
fi

if ! grep -i --quiet HOOT /etc/postgresql/9.1/main/postgresql.conf; then
echo "### Tuning PostgreSQL..."
sudo -u postgres sed -i.bak s/^max_connections/\#max_connections/ /etc/postgresql/9.1/main/postgresql.conf
sudo -u postgres sed -i.bak s/^shared_buffers/\#shared_buffers/ /etc/postgresql/9.1/main/postgresql.conf
sudo -u postgres bash -c "cat >> /etc/postgresql/9.1/main/postgresql.conf" <<EOT

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

# Configure Tomcat

if ! grep --quiet TOMCAT6_HOME ~/.profile; then
    echo "### Adding Tomcat to profile..."
    #echo "export TOMCAT6_HOME=/usr/share/tomcat6" >> ~/.profile
    echo "export TOMCAT6_HOME=/var/lib/tomcat6" >> ~/.profile
    source ~/.profile
fi

cd $HOOT_HOME
source ./SetupEnv.sh

if [ ! "$(ls -A hoot-ui)" ]; then
    echo "hoot-ui is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

if ! grep -i --quiet HOOT /etc/default/tomcat6; then
echo "Configuring tomcat6 environment..."
# This echo properly substitutes the home path dir and keeps it from having to be hardcoded, but 
# fails on permissions during write...so hardcoding the home path here instead for now.  This 
# hardcode needs to be removed in order for hoot dev env install script to work correctly.
#
#sudo echo "#--------------
# Hoot Settings
#--------------
#HOOT_HOME=\$HOOT_HOME/hoot" >> /etc/default/tomcat6

sudo bash -c "cat >> /etc/default/tomcat6" <<EOT
#--------------
# Hoot Settings
#--------------
HOOT_HOME=/home/vagrant/hoot
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib
GDAL_DATA=/usr/local/share/gdal
GDAL_LIB_DIR=/usr/local/lib
HOOT_WORKING_NAME=hoot
PATH=$HOOT_HOME/bin:$PATH
EOT
fi

if ! grep -i --quiet 'umask 002' /etc/default/tomcat6; then
echo "### Changing Tomcat umask to group write..."
sudo bash -c "cat >> /etc/default/tomcat6" <<EOT
# Set tomcat6 umask to group write because all files in shared folder are owned by vagrant
umask 002
EOT
fi

if grep -i --quiet '^JAVA_OPTS=.*\-Xmx128m' /etc/default/tomcat6; then
    echo "### Changing Tomcat java opts..."
    sudo sed -i.bak "s@\-Xmx128m@\-Xms512m \-Xmx2048m \-XX:PermSize=512m \-XX:MaxPermSize=4096m@" /etc/default/tomcat6
fi

if grep -i --quiet 'gdal/1.10' /etc/default/tomcat6; then
    echo "### Fixing Tomcat GDAL_DATA env var path..."
    sudo sed -i.bak s@^GDAL_DATA=.*@GDAL_DATA=\/usr\/local\/share\/gdal@ /etc/default/tomcat6
fi

# Remove gdal libs installed by libgdal-dev that interfere with
# renderdb-export-server using gdal libs compiled from source (fgdb support)
if [ -f "/usr/lib/libgdal.*" ]; then
    echo "Removing GDAL libs installed by libgdal-dev..."
    sudo rm /usr/lib/libgdal.*
fi

if ! grep -i --quiet 'ingest/processed' /etc/tomcat6/server.xml; then
    echo "Adding Tomcat context path for tile images..."
    sudo sed -i.bak 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/ingest\/processed\" path=\"\/static\" \/>\n      &@' /etc/tomcat6/server.xml
fi

if ! grep -i --quiet 'allowLinking="true"' /etc/tomcat6/context.xml; then
    echo "Set allowLinking to true in Tomcat context..."
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" /etc/tomcat6/context.xml
fi

if [ ! -d /usr/share/tomcat6/.deegree ]; then
    echo "Creating deegree directory for webapp..."
    sudo mkdir /usr/share/tomcat6/.deegree
    sudo chown tomcat6:tomcat6 /usr/share/tomcat6/.deegree
fi

if [ -f $HOOT_HOME/conf/LocalHoot.json ]; then
    echo "Removing LocalHoot.json..."
    rm -f $HOOT_HOME/conf/LocalHoot.json
fi

if [ -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf ]; then
    echo "Removing services local.conf..."
    rm -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf
fi

# Update marker file date now that dependency and config stuff has run
# The make command will exit and provide a warning to run 'vagrant provision'
# if the marker file is older than this file (VagrantProvision.sh)
touch Vagrant.marker

mkdir -p $HOOT_HOME/ingest/processed
mkdir -p $HOOT_HOME/upload
# This is a workaround.
sudo chmod -R 777 $HOOT_HOME/ingest
sudo chmod -R 777 $HOOT_HOME/upload
sudo chmod -R 777 $HOOT_HOME/tmp

# Now we are ready to build Hoot.  The VagrantBuild.sh script will build Hoot.

