#!/usr/bin/env bash

# Update OS
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get dist-upgrade -y
sudo apt-get install ntp -y

# Add the postgres repo
if [ ! -f /etc/apt/sources.list.d/pgdg.list ]; then
    echo "Adding PostgreSQL repository to apt"
    sudo bash -c "echo 'deb http://apt.postgresql.org/pub/repos/apt/ '$(lsb_release -cs)'-pgdg main' > /etc/apt/sources.list.d/pgdg.list"
    wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
    sudo apt-get update -y
    sudo apt-get upgrade -y
fi

# Install dependencies
echo "Installing dependencies"
sudo apt-get install -y texinfo g++ libicu-dev libqt4-dev git-core libboost-dev libcppunit-dev libcv-dev libopencv-dev libgdal-dev liblog4cxx10-dev libnewmat10-dev libproj-dev python-dev libjson-spirit-dev automake1.11 protobuf-compiler libprotobuf-dev gdb libqt4-sql-psql libgeos++-dev swig lcov tomcat6 openjdk-7-jdk openjdk-7-dbg maven libstxxl-dev nodejs-dev nodejs-legacy doxygen xsltproc asciidoc pgadmin3 curl npm libxerces-c28 libglpk-dev libboost-all-dev source-highlight texlive-lang-all graphviz w3m python-setuptools python python-pip git ccache libogdi3.2-dev gnuplot python-matplotlib libqt4-sql-sqlite

# Hoot Baseline is PostgreSQL 9.1 and PostGIS 1.5, so we need a deb file and
# then remove 9.4
if [ ! -f postgresql-9.1-postgis_1.5.3-2_amd64.deb ]; then
    echo "Installing PostgreSQL 9.1"
    wget http://launchpadlibrarian.net/86690107/postgresql-9.1-postgis_1.5.3-2_amd64.deb
    sudo dpkg -i postgresql-9.1-postgis_1.5.3-2_amd64.deb
    sudo apt-get -f install -y
    # fixes missing dependency of postgis 1.5 by installing postgresql 9.1. 9.1 is installed listening on the default port, 5432. It unfortunately also installs postgres 9.4 but we remove that cleanly in the following steps, while leaving postgres 9.1 untouched
    echo "Removing PostgreSQL 9.4"
    sudo apt-get purge -y postgresql-9.4 postgresql-client-9.4 postgresql-9.4-postgis-scripts
    sudo apt-get install -y postgresql-contrib-9.1
fi

# Get GDAL & FileGDB_API source
if [ ! -f gdal-1.10.1.tar.gz ]; then
    echo "Downloading GDAL source"
    wget http://download.osgeo.org/gdal/1.10.1/gdal-1.10.1.tar.gz
fi
if [ ! -d gdal-1.10.1 ]; then
    echo "Extracting GDAL source"
    tar zxvfp gdal-1.10.1.tar.gz
fi
if [ ! -f FileGDB_API_1_3-64.tar.gz ]; then
    echo "Downloading FileGDB API source"
    wget http://downloads2.esri.com/Software/FileGDB_API_1_3-64.tar.gz
fi
if [ ! -d /usr/local/FileGDB_API ]; then
    echo "Extracting FileGDB API source & installing lib"
    sudo tar xvfp FileGDB_API_1_3-64.tar.gz --directory /usr/local
    sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
fi

# Build GDAL w/ FileGDB
if ! ogrinfo --formats | grep --quiet FileGDB; then
    echo "Building GDAL w/ FileGDB"
    export PATH=/usr/local/lib:/usr/local/bin:$PATH
    cd gdal-1.10.1
    sudo ./configure --with-fgdb=/usr/local/FileGDB_API --with-pg=/usr/bin/pg_config --with-python
    sudo make -j5
    sudo make install
    cd swig/python
    python setup.py build
    sudo python setup.py install
    sudo ldconfig
    cd ~
fi

# Node.js modules
if ! grep --quiet NODE_PATH ~/.profile; then
    echo "Installing node js dependencies"
    sudo npm config set registry http://registry.npmjs.org/
    sudo npm install -g xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler
    echo 'Adding NODE_PATH to user environment'
    echo 'export NODE_PATH=/usr/local/lib/node_modules' >> ~/.profile
    source ~/.profile
fi

# Create Services Database
if ! sudo -u postgres psql -lqt | grep -i --quiet hoot; then
    echo "Creating Services Database"
    sudo -u postgres createuser --superuser hoot
    sudo -u postgres psql -c "alter user hoot with password 'hoottest';"
    sudo -u postgres createdb hoot --owner=hoot
    sudo -u postgres createdb wfsstoredb --owner=hoot
    sudo -u postgres psql -d hoot -c 'create extension hstore;'
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'"
    sudo -u postgres psql -d wfsstoredb -f /usr/share/postgresql/9.1/contrib/postgis-1.5/postgis.sql
    sudo -u postgres psql -d wfsstoredb -f /usr/share/postgresql/9.1/contrib/postgis-1.5/spatial_ref_sys.sql
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geometry_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geography_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on spatial_ref_sys TO PUBLIC;"
fi

# PostgreSQL Tuning
if ! grep -i --quiet HOOT /etc/postgresql/9.1/main/postgresql.conf; then
echo "Tuning PostgreSQL"
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
checkpoint_segments = 20
autovacuum = off
EOT
fi

# Update shared memory limits in OS
if ! sysctl -e kernel.shmmax | grep --quiet 1173741824; then
    echo "Setting kernel.shmmax"
    sudo sysctl -w kernel.shmmax=1173741824
    sudo sh -c "echo 'kernel.shmmax=1173741824' >> /etc/sysctl.conf"
fi
if ! sysctl -e kernel.shmall | grep --quiet 2097152; then
    echo "Setting kernel.shmall"
    sudo sysctl -w kernel.shmall=2097152
    sudo sh -c "echo 'kernel.shmall=2097152' >> /etc/sysctl.conf"
fi

# Restart PostgreSQL
sudo service postgresql restart

# Configure and Build
cd /home/vagrant/hoot
cp ./conf/DatabaseConfig.sh.orig ./conf/DatabaseConfig.sh
source ./SetupEnv.sh

echo "Configuring Hoot"
aclocal && autoconf && autoheader && automake && ./configure -q --with-rnd --with-services
if [ ! -f LocalConfig.pri ] && ! grep --quiet QMAKE_CXX LocalConfig.pri; then
    echo 'Customizing LocalConfig.pri'
    cp LocalConfig.pri.orig LocalConfig.pri
    echo 'QMAKE_CXX=ccache g++' >> LocalConfig.pri
fi
echo "Building Hoot"
make clean
make -sj4
make docs

# Tweak dev environment to make tests run faster
# FIXME: make this command not destructive to local.conf
echo 'testJobStatusPollerTimeout=250' > $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf

# Run Tests
#echo "Running tests"
#make -sj4 test
#make -sj4 test-all

# Deploy to Tomcat
if ! grep -i --quiet HOOT /etc/default/tomcat6; then
echo "Configuring tomcat6 environment"
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

# Change Tomcat umask to group write
if ! grep -i --quiet 'umask 002' /etc/default/tomcat6; then
echo "Changing Tomcat umask to group write"
sudo bash -c "cat >> /etc/default/tomcat6" <<EOT
# Set tomcat6 umask to group write because all files in shared folder are owned by vagrant
umask 002
EOT
fi

# Fix env var path for GDAL_DATA
if grep -i --quiet 'gdal/1.10' /etc/default/tomcat6; then
    echo "Fixing Tomcat GDAL_DATA env var path"
    sudo sed -i.bak s@^GDAL_DATA=.*@GDAL_DATA=\/usr\/local\/share\/gdal@ /etc/default/tomcat6
fi

# Remove gdal libs installed by libgdal-dev that interfere with
# mapedit-export-server using gdal libs compiled from source (fgdb support)
if [ -f /usr/lib/libgdal.* ]; then
    echo "Removing GDAL libs installed by libgdal-dev"
    sudo rm /usr/lib/libgdal.*
fi

# Create Tomcat context path for tile images
if ! grep -i --quiet 'ingest/processed' /etc/tomcat6/server.xml; then
    echo "Adding Tomcat context path for tile images"
    sudo sed -i.bak "s@<\/Host>@  <Context docBase=\"\/home\/vagrant\/hoot\/ingest\/processed\" path=\"\/static\" \/>\n      &@" /etc/tomcat6/server.xml
fi

# Allow linking in Tomcat context
if ! grep -i --quiet 'allowLinking="true"' /etc/tomcat6/context.xml; then
    echo "Set allowLinking to true in Tomcat context"
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" /etc/tomcat6/context.xml
fi

# Deploy to Tomcat
echo "Stopping Tomcat"
sudo service tomcat6 stop
echo "Deploying Hoot to Tomcat"
sudo -u tomcat6 scripts/vagrantDeployTomcat.sh
echo "Starting Tomcat"
sudo service tomcat6 start

# Create directory for webapp
if [ ! -d /usr/share/tomcat6/.deegree ]; then
    echo "Creating directory for webapp"
    sudo mkdir /usr/share/tomcat6/.deegree
    sudo chown tomcat6:tomcat6 /usr/share/tomcat6/.deegree
fi
