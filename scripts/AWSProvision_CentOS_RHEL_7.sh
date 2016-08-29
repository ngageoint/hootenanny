#!/usr/bin/env bash

sudo sh -c "echo '127.1.0.0 ${HOSTNAME}' >> /etc/hosts"

# Prompt user to run script as root
if [ "$EUID" -ne 0 ] ; then
  echo "Please run this script as root"
  exit -1;
fi

# Prompt User what Release they intend to Deploy
echo -n "Please enter HOOT_VERSION for install (x.y.z): " && read HOOT_VERSION

if [ -z ${HOOT_VERSION} ] ; then
    echo "ERROR: HOOT_VERSION can not be empty"
    exit -1;
fi

sudo sh -c "echo 'export HOOT_HOME_RPMS=/home/${USER}/rpms' >> /etc/profile"
sudo sh -c "echo 'export CORE_INSTALL_USER_GROUP=root' >> /etc/profile"
sudo sh -c "echo 'export HOOT_CORE_INSTALL_DIR=/usr/local' >> /etc/profile"
sudo sh -c "echo 'export HOOT_HOME=/usr/local/hoot' >> /etc/profile"
sudo sh -c "echo 'export M2_HOME=/usr/local/maven' >> /etc/profile"
source /etc/profile

# Testing RPM location
HOOT_HOME_RPMS=/home/${USER}/rpms
echo "HOOT_HOME_RPMS: " ${HOOT_HOME_RPMS}
# the user group used when installing hoot core; needed for compiling hoot core only; The user
# executing this script must be a member of this group, and the group must have the correct
# privileges to install files to HOOT_CORE_INSTALL_DIR.
CORE_INSTALL_USER_GROUP=root
echo "CORE_INSTALL_USER_GROUP: " ${CORE_INSTALL_USER_GROUP}
# Where the Hootenanny core application is installed to; usually: ${HOOT_CORE_INSTALL_DIR}
HOOT_CORE_INSTALL_DIR=/usr/local
echo "HOOT_CORE_INSTALL_DIR: " ${HOOT_CORE_INSTALL_DIR}

sudo sh -c "echo 'export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOOT_CORE_INSTALL_DIR}/lib:${HOOT_HOME}/lib:${HOOT_HOME}/pretty-pipes/lib:${LD_LIBRARY_PATH}:${M2_HOME}/bin' >> /etc/profile"
source /etc/profile

if [ ${USER} == 'root' ] ; then
	sudo sh -c "echo 'export HOOT_HOME_RPMS=/${USER}/rpms' >> /etc/profile"
	source /etc/profile
fi

# Enable all Centos Repos
sudo yum-config-manager --enable *

# Start at home
cd ~

# Install wget and get
echo "installing wget and git"
yum -y install wget git

test -f centos-6.x-rpms.tar || wget https://github.com/ngageoint/hootenanny-rpms/releases/download/v0.1.0/centos-6.x-rpms.tar
echo "Extracting RPMs"
tar -xvf ~/centos-6.x-rpms.tar

# Download latest epel release 7.5
wget https://dl.fedoraproject.org/pub/epel/7/x86_64/e/epel-release-7-5.noarch.rpm

mv epel-release-7-5.noarch.rpm ~/rpms/
sudo rpm -ivh ${HOOT_HOME_RPMS}/epel-release-7-5.noarch.rpm


# Install everything from the yum repo where hoot is ok with using the latest version.
echo "Installing packages from yum..."
sudo yum -y install gcc gcc-c++ libicu-devel boost-devel python-devel gdb cmake swig ccache
sudo yum -y install glpk glpk-devel apr-devel apr-util-devel nodejs-devel nodejs v8-devel qt qt-devel
sudo yum -y install fontconfig-devel libX11-devel libXcursor-devel libXext-devel libXfixes libXft-devel
sudo yum -y install freetype-devel java-1.7.0-openjdk-devel tomcat tomcat-admin-webapps tomcat-webapps unixODBC-devel.x86_64 uuid.x86_64
sudo yum -y install autoconf libxslt log4cxx-devel poppler python-requests ruby teckit tex-preview xdvipdfmx texlive-dvipng texlive-dvipdfm
sudo yum -y install geos geos-devel hdf5 gdal json-c proj proj-devel npm unzip doxygen nano gd-devel libXau-devel libXpm-devel libjpeg-turbo-devel libxcb-devel
sudo yum -y install xorg-x11-proto-devel libgfortran libXi-devel libXrandr-devel libXrender-devel texlive-kpathsea python-matplotlib
sudo yum -y install automake maven cppunit cppunit-devel blas lapack opencv opencv-devel opencv-python protobuf protobuf-devel texlive python-dateutil
sudo yum -y install python-setuptools gnuplot gnuplot-latex gnuplot-common libpng libpng-devel asciidoc dblatex numpy python-numexpr perl-CPAN libtool

# Install python packages via easy_install
sudo easy_install argparse

echo "Installing reporting packages..."
sudo rpm -Uvh ${HOOT_HOME_RPMS}/msttcore-fonts-2.0-3.noarch.rpm
if ! grep --quiet QT_HOME /etc/profile; then
  echo 'export GDFONTPATH=/usr/share/fonts/msttcore' >> /etc/profile && source /etc/profile
fi

echo "Installing database packages and configuring database..."
sudo yum install -y postgresql-libs postgresql postgresql-server postgresql-contrib postgresql-devel
sudo postgresql-setup initdb
sudo service postgresql start

# set Postgres to autostart
sudo systemctl enable postgresql
sudo yum install -y postgis liquibase

sleep 10

# Create Services Database
echo "Creating Services Database"
sudo -u postgres createuser --superuser hoot
sudo -u postgres psql -c "alter user hoot with password 'hoottest';"
sudo -u postgres createdb hoot --owner=hoot
sudo -u postgres createdb wfsstoredb --owner=hoot
sudo -u postgres psql -d hoot -c 'create extension hstore;'
sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'"
sudo -u postgres psql -d wfsstoredb -f /usr/share/pgsql/contrib/postgis-64.sql  # New location of postgis.sql file?
sudo -u postgres psql -d wfsstoredb -f /usr/share/pgsql/contrib/postgis-2.0/spatial_ref_sys.sql
sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geometry_columns TO PUBLIC;"
sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geography_columns TO PUBLIC;"
sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on spatial_ref_sys TO PUBLIC;"

# PostgreSQL Tuning
#if ! grep -i --quiet HOOT /var/lib/pgsql/data/postgresql.conf; then
   echo "Tuning PostgreSQL"
   sudo -u postgres sed -i.bak s/^max_connections/\#max_connections/ /var/lib/pgsql/data/postgresql.conf
   sudo -u postgres sed -i.bak s/^shared_buffers/\#shared_buffers/ /var/lib/pgsql/data/postgresql.conf
   sudo -u postgres bash -c "cat >> /var/lib/pgsql/data/postgresql.conf" <<EOT
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
#fi

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

# update pg_hba.conf to trust authentication
if ! sudo grep -i --quiet HOOT /var/lib/pgsql/data/pg_hba.conf; then
  sudo -u postgres sed -i.bak s/peer/trust/ /var/lib/pgsql/data/pg_hba.conf
  sudo -u postgres sed -i.bak s/ident/trust/ /var/lib/pgsql/data/pg_hba.conf
fi

# Restart PostgreSQL
sudo service postgresql restart


echo "Installing FileGDB"
sudo rm -rf ${HOOT_CORE_INSTALL_DIR}/FileGDB_API-1.3
sudo tar -xzf ${HOOT_HOME_RPMS}/FileGDB_API_1_3-64.tar.gz --directory ${HOOT_CORE_INSTALL_DIR}
sudo chown -R root:${CORE_INSTALL_USER_GROUP} ${HOOT_CORE_INSTALL_DIR}/FileGDB_API
sudo cp -R ${HOOT_CORE_INSTALL_DIR}/FileGDB_API ${HOOT_CORE_INSTALL_DIR}/FileGDB_API-1.3
sudo cp ${HOOT_CORE_INSTALL_DIR}/FileGDB_API-1.3/lib/* ${HOOT_CORE_INSTALL_DIR}/lib/

echo "Installing GDAL..."
sudo rpm -ivh ${HOOT_HOME_RPMS}/gdal-1.10.1_x86_64.rpm

if ! grep --quiet GDAL_LIB_DIR /etc/profile; then
   echo 'export GDAL_LIB_DIR=/usr/local/lib' >> /etc/profile && source /etc/profile
fi
if ! grep --quiet GDAL_DATA /etc/profile; then
   echo 'export GDAL_DATA=/usr/local/share/gdal' >> /etc/profile && source /etc/profile
fi

# Setup symlink for qmake
sudo rm -f /usr/bin/qmake
sudo ln -s /usr/lib64/qt4/bin/qmake /usr/bin/qmake

if ! grep --quiet QT_HOME /etc/profile; then
	echo 'export QT_HOME=/usr/lib64/qt4/' >> /etc/profile && source /etc/profile
fi

# Node.js modules
if ! grep --quiet NODE_PATH /etc/profile; then
    echo "Installing node js dependencies"
    sudo npm config set registry http://registry.npmjs.org/
    sudo npm install -g xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler
    echo 'Adding NODE_PATH to user environment'
    echo 'export NODE_PATH=/usr/lib/node_modules' >> /etc/profile && source /etc/profile
fi

# Module needed for OSM API db test
if [ ! -d ${HOME}/.cpan ]; then
    (echo y;echo o conf prerequisites_policy follow;echo o conf commit)|sudo cpan
    sudo perl -MCPAN -e 'install XML::Simple'
fi

# Configure Tomcat
if ! sudo grep -i --quiet HOOT /etc/default/tomcat; then
echo "Configuring tomcat environment"
sudo bash -c "cat >> /etc/default/tomcat" <<EOT
#--------------
# Hoot Settings
#--------------
HOOT_HOME=/usr/local/hoot
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib
GDAL_DATA=/usr/local/share/gdal
GDAL_LIB_DIR=/usr/local/lib
HOOT_WORKING_NAME=hoot
QT_HOME=/usr/lib64/qt4/
#THIS IS THE CORRECT PATH FOR PSQL_HOME? IS IT NEEDED?
PSQL_HOME=/usr/share/pgsql
PATH=$HOOT_HOME/bin:$PATH
EOT
fi

# Create Tomcat context path for tile images
  if ! grep -i --quiet 'ingest/processed' /etc/tomcat/server.xml; then
    echo "Adding Tomcat context path for tile images"
    sudo sed -i.bak "s@<\/Host>@ <Context docBase=\"\/usr\/local\/hoot\/ingest\/processed\" path=\"\/static\" \/>\n &@" /etc/tomcat/server.xml
  fi

# Allow linking in Tomcat context
if ! grep -i --quiet 'allowLinking="true"' /etc/tomcat/context.xml; then
    echo "Set allowLinking to true in Tomcat context"
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" /etc/tomcat/context.xml
fi

# Create directory for webapp
if [ ! -d /usr/share/tomcat/.deegree ]; then
    echo "Creating directory for webapp"
    sudo mkdir /usr/share/tomcat/.deegree
    sudo chown tomcat:tomcat /usr/share/tomcat/.deegree
fi

# Set Tomcat to autostart
sudo systemctl enable tomcat
sudo service tomcat restart

echo "Configuring path..."
  JAVA_HOME=/usr/lib/jvm/`ls /usr/lib/jvm | grep -e java-1.7.*-openjdk-*.x86_64`
  if ! sudo grep --quiet 'JAVA_HOME' /etc/profile; then
    sudo sh -c "echo 'export JAVA_HOME='${JAVA_HOME} >> /etc/profile"
    source /etc/profile
  fi
  if ! sudo grep --quiet 'export PATH=' /etc/profile; then
    sudo sh -c "echo 'export PATH=${HOOT_CORE_INSTALL_DIR}/lib:${HOOT_CORE_INSTALL_DIR}/bin:${PATH}:${QT_HOME}/bin:${PSQL_HOME}/bin:${M2_HOME}/bin:' >> /etc/profile"
    source /etc/profile
  fi

# Update the init.d script for node-mapnik-server
#sudo cp ${HOOT_HOME}/node-mapnik-server/init.d/node-mapnik-server /etc/init.d
#sudo chmod a+x /etc/init.d/node-mapnik-server
# Make sure all npm modules are installed
#cd ${HOOT_HOME}/node-mapnik-server
#sudo npm install
#cd ..

# At this point of the script, hoot dependencies should be installed.
# Do some clean up work to free up space
echo "Checking Disk Space..."
FREE=$(df -h . | awk 'NR==2 {print $5}' | cut -d'%' -f1)

if [[ ${FREE} -gt 70 ]]; then
  echo "Used Capacity exceeding 70%."
  echo "Clearing up space..."
  rm -rf ~/*.tar*
  rm -rf ~/rpms/

  FREE=$(df -h . | awk 'NR==2 {print $5}' | cut -d'%' -f1)

  if [[ ${FREE} -lt 80 ]]; then
	echo "Used Capacity has dropped below 80%"
  else
	echo "Not enough space on device to install Hoot. Please manually remove files to create space"
	exit 1
  fi
fi

cd ${HOOT_CORE_INSTALL_DIR}/
source /etc/profile

# PULL RELEASE CANDIDATE FROM GITHUB
test -f hootenanny-${HOOT_VERSION}.tar.gz || sudo wget https://github.com/ngageoint/hootenanny/releases/download/v${HOOT_VERSION}/hootenanny-${HOOT_VERSION}.tar.gz
test -f hootenanny-services-${HOOT_VERSION}.war || sudo wget https://github.com/ngageoint/hootenanny/releases/download/v${HOOT_VERSION}/hootenanny-services-${HOOT_VERSION}.war
sudo tar -xzf hootenanny-${HOOT_VERSION}.tar.gz # This extracts to hootenanny-0.2.20

# setup symlink for $HOOT_HOME = /usr/local/hoot
sudo ln -s ${HOOT_CORE_INSTALL_DIR}/hootenanny-${HOOT_VERSION} ${HOOT_CORE_INSTALL_DIR}/hoot

# Ensure Hoot database has tables loaded
echo "Updating Hoot Database"
sudo mkdir -p ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-temp
sudo cp ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}.war ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-temp
cd ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-temp
unzip -q hootenanny-services-${HOOT_VERSION}.war
cd ..
sudo mv ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-temp ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}

# liquibase bases its decision on whether to apply an update partially on what
# directory each changeset XML file resides in.  Therefore, you must execute
# this command exactly as written, from the exact directory shown here.  If not,
# unnecessary updates will be applied, which will result in SQL errors.
cd ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}/WEB-INF
sudo liquibase --contexts=default,production --changeLogFile=classes/db/db.changelog-master.xml --promptForNonLocalDatabase=false --defaultsFile=classes/db/liquibase.properties --logLevel=warning --classpath=lib/postgresql-9.4.1208.jre7.jar --url jdbc:postgresql:hoot update #&> /usr/local/test
cd ${HOOT_CORE_INSTALL_DIR}/
sudo rm -rf hootenanny-services-${HOOT_VERSION}

echo "hoot database updated. Restarting Database"
sudo service postgresql restart
sleep 10

# go inside Hoot to build
cd ${HOOT_HOME}
source ./SetupEnv.sh

# Create empty ChangeLog file
touch ChangeLog

echo "Configuring Hoot..."
aclocal && autoconf && autoheader && automake

echo "./configure: ./configure --with-rnd"
./configure -q --with-rnd

echo "Building Hoot..."
make clean -sj$(nproc)
make -sj$(nproc)
#make docs -sj$(nproc)

#Copy docs from github into /usr/local/hoot/docs/ directory
wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ Algorithms.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.Algorithms.pdf
wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ Developer\ Guide.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.Developer.Guide.pdf
wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ Installation\ Instructions.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.Installation.Instructions.pdf
wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ User\ Guide.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.User.Guide.pdf
wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ User\ Interface\ Guide.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.User.Interface.Guide.pdf

# Run Tests
echo "Running tests"
make -sj$(nproc) test
make -sj$(nproc) test-all

# configure hoot core permissions
sudo chmod -R 755 ${HOOT_HOME}
sudo chown -R tomcat:tomcat ${HOOT_HOME}
sudo chmod -R 755 ${HOOT_CORE_INSTALL_DIR}/hootenanny-${HOOT_VERSION}
sudo chown -R tomcat:tomcat ${HOOT_CORE_INSTALL_DIR}/hootenanny-${HOOT_VERSION}
sudo ln -s ${HOOT_CORE_INSTALL_DIR}/hoot/bin/hoot ${HOOT_CORE_INSTALL_DIR}/bin/hoot

echo "Hoot Version"
hoot version
sleep 3

# Deploy to Tomcat
echo "Deploying Hoot to Tomcat"
sudo sh -c "echo 'export TOMCAT_HOME=/var/lib/tomcat/webapps' >> /etc/profile"
source /etc/profile

sudo chmod 775 ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}.war
sudo chown tomcat:tomcat ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}.war

sudo -u tomcat rm -f ${TOMCAT_HOME}/hoot-services-*.war
sudo -u tomcat cp ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}.war ${TOMCAT_HOME}/hoot-services.war
sudo -u tomcat rm -rf ${TOMCAT_HOME}/hootenanny-id/
sudo -u tomcat cp -R ${HOOT_CORE_INSTALL_DIR}/hootenanny-${HOOT_VERSION}/hoot-ui ${TOMCAT_HOME}/hootenanny-id
sudo -u tomcat rm -rf ${TOMCAT_HOME}/hootenanny-id/css/img
sudo -u tomcat cp -Rf ${TOMCAT_HOME}/hootenanny-id/dist/img ${TOMCAT_HOME}/hootenanny-id/css/

echo "Wait for tomcat to extract the hoot-services.war file..."
sleep 10

# set web services permissions
sudo chown -R tomcat:tomcat ${TOMCAT_HOME}/hoot-services
sudo chmod -R 775 ${TOMCAT_HOME}/hoot-services
# needed by WFS
sudo chmod -R 777 ${TOMCAT_HOME}/hoot-services/WEB-INF/workspace

# add folders and permissions for web app
sudo mkdir -p ${HOOT_HOME}/ingest/processed/
sudo chmod -R 777 ${HOOT_HOME}/ingest

echo "Restarting tomcat server..."
sudo service tomcat restart
sleep 10
echo "Tomcat Server restarted"

cd ${HOOT_HOME}
source SetupEnv.sh

#Start nodejs
nohup npm start

echo "Hootenanny install of version ${HOOT_VERSION} has been completed.  Access the command line interface by typing: 'hoot'"
echo "View the web interface at http://localhost:8080/hootenanny-id/"
