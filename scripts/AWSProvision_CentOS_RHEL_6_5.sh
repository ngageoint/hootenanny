#!/bin/bash

# AWS
sudo sh -c "echo '127.1.0.0 ${HOSTNAME}' >> /etc/hosts"

# This Script is used to install Hootenanny dependencies and the Hootenanny Application
# To Note: This script will only work for CentOS/RHEL 6.5
# Updates to the script will need to be made to handle newer versions of CentOS/RHEL 6.7 OS and 3rd party libraries

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

  echo "Downloading CentOS 6.x RPMS..."

  # Install wget and get
  echo "installing wget and git"
  yum -y install wget git

  # Install Maven
  echo "Downloading and installing Maven..."
  wget http://mirror.cc.columbia.edu/pub/software/apache/maven/maven-3/3.0.5/binaries/apache-maven-3.0.5-bin.tar.gz
  sudo tar xzf apache-maven-3.0.5-bin.tar.gz -C /usr/local
  cd /usr/local
  sudo ln -s apache-maven-3.0.5 maven

  # go back home
  cd ~

  test -f centos-6.x-rpms.tar || wget https://github.com/ngageoint/hootenanny-rpms/releases/download/v0.1.0/centos-6.x-rpms.tar
  echo "Extracting RPMs"
  tar -xvf ~/centos-6.x-rpms.tar

  echo "Install 6.8 epel repo"
  sudo rpm -ivh ${HOOT_HOME_RPMS}/epel-release-6-8.noarch.rpm

  # remove old gnuplot
  echo "remove gnuplot libs"
  sudo yum erase -y gnuplot gnuplot-latex gnuplot-common

  # Install everything from the yum repo where hoot is ok with using the latest version.
  echo "Installing packages from yum..."
  sudo yum -y install gcc gcc-c++ libicu-devel boost-devel python-devel gdb cmake python-numeric swig
  sudo yum -y install glpk glpk-devel python-argparse apr-devel apr-util-devel nodejs-devel nodejs v8-devel
  sudo yum -y install libX11-devel fontconfig-devel libXcursor-devel libXext-devel libXfixes libXft-devel libXi-devel libXrandr-devel libXrender-devel
  sudo yum -y install java-1.7.0-openjdk-devel unixODBC-devel.x86_64 uuid.x86_64 tomcat6 tomcat6-webapps tomcat6-admin-webapps freetype-devel
  sudo yum -y install wget autoconf libxslt poppler kpathsea xdvipdfmx dvipdfm dvipng teckit perl-PDF-Reuse ruby tex-preview python-requests log4cxx-devel
  sudo yum -y install geos hdf5 json-c proj gdal npm unzip doxygen nano gd-devel libX11-devel libXau-devel libXpm-devel libjpeg-turbo-devel libxcb-devel
  sudo yum -y install xorg-x11-proto-devel libgfortran automake

  sudo yum -y install python-argparse || export ARGPARSE_FAILED=1
  if [ "$ARGPARSE_FAILED" == "1" ]; then
    sudo yum -y install python-setuptools
    easy_install argparse
  fi

  #autoconf 2.69
  echo "download autoconf 2.69"
  wget http://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz

  mv autoconf-2.69.tar.gz ${HOOT_HOME_RPMS}

  echo "extract and build autoconf"
  tar xzf ${HOOT_HOME_RPMS}/autoconf-2.69.tar.gz
  cd autoconf-2.69
  ./configure --prefix=${HOOT_CORE_INSTALL_DIR}
  make && sudo make install
  export PATH=$PATH:${HOOT_CORE_INSTALL_DIR}/bin

  echo "install packages built from source as rpms..."
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/cppunit-1.12.1-1.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/proj-4.8.0-1.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/geos-3.3.8-1.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/blas-3.2.1-4.el6.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/lapack-3.2.1-4.el6.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/python-numpy-1.6.1-17.1.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/python-numpy-devel-1.6.1-17.1.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/opencv-2.4.5-1.x86_64.rpm
  sudo rpm -ifvh ${HOOT_HOME_RPMS}/protobuf-2.4.1-1.x86_64.rpm

  echo "Installing reporting packages..."
  sudo rpm -Uvh ${HOOT_HOME_RPMS}/msttcore-fonts-2.0-3.noarch.rpm
  if ! sudo grep --quiet QT_HOME /etc/profile; then
    sudo sh -c "echo 'export GDFONTPATH=/usr/share/fonts/msttcore' >> /etc/profile"
    source /etc/profile
  fi

  echo "Install more reporting packages - gnuplot and texlive..."
  sudo rpm -ivh ${HOOT_HOME_RPMS}/gnuplot-common-4.2.6-2.el6.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/gnuplot-4.2.6-2.el6.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/gnuplot-latex-4.2.6-2.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/gnuplot-4.6.4-1.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/libpng-1.2.49-1.el6_2.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/libpng-devel-1.2.49-1.el6_2.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-2007-38.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-fonts-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-dvips-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-dvips-2007-38.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-fonts-2007-38.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-afm-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-afm-2007-38.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-xetex-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-doc-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-doc-2007-38.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-context-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-context-2007-38.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-errata-latex-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-afm-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-context-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-texmf-xetex-2007-38.el6.noarch.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-xetex-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/texlive-dviutils-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/asciidoc-8.6.9-1.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/dblatex-0.2.8-1.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/python-numpy-1.6.1-17.1.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/python-numpy-devel-1.6.1-17.1.x86_64.rpm

# Dependencies needed for Matplotlib
  sudo yum -y install python-dateutil.noarch

  sudo sh -c "echo 'export PYTHONPATH=${HOOT_CORE_INSTALL_DIR}/matplotlib-1.3.1/lib64/python2.6/site-packages' >> /etc/profile"
  sudo rpm -ivh ${HOOT_HOME_RPMS}/matplotlib-1.3.1_x86_64.rpm

  echo "Installing database packages and configuring database..."
  sudo rpm -ivh ${HOOT_HOME_RPMS}/postgresql91-libs-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/postgresql91-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/postgresql91-server-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/postgresql91-contrib-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/postgresql91-devel-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo service postgresql-9.1 initdb
  sudo service postgresql-9.1 start

  # set Postgres to autostart
  sudo /sbin/chkconfig --add postgresql-9.1
  sudo /sbin/chkconfig postgresql-9.1 on

  sudo rpm -ivh ${HOOT_HOME_RPMS}/postgis2_91-2.0.4-1.rhel6.x86_64.rpm
  sudo rpm -ivh ${HOOT_HOME_RPMS}/liquibase-3.1.0-1.el6.noarch.rpm
	
  sleep 10

  echo "Creating Services Database"
  sudo -u postgres createuser --superuser hoot
  sudo -u postgres psql -c "alter user hoot with password 'hoottest';"
  sudo -u postgres createdb hoot --owner=hoot
  sudo -u postgres createdb wfsstoredb --owner=hoot
  sudo -u postgres psql -d hoot -c 'create extension hstore;'
  sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'"
  sudo -u postgres psql -d wfsstoredb -f /usr/pgsql-9.1/share/contrib/postgis-2.0/postgis.sql
  sudo -u postgres psql -d wfsstoredb -f /usr/pgsql-9.1/share/contrib/postgis-2.0/spatial_ref_sys.sql
  sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geometry_columns TO PUBLIC;"
  sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geography_columns TO PUBLIC;"
  sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on spatial_ref_sys TO PUBLIC;"

  if ! sudo grep -i --quiet HOOT /var/lib/pgsql/9.1/data/postgresql.conf; then
    echo "Tuning PostgreSQL"
    sudo -u postgres sed -i.bak s/^max_connections/\#max_connections/ /var/lib/pgsql/9.1/data/postgresql.conf
    sudo -u postgres sed -i.bak s/^shared_buffers/\#shared_buffers/ /var/lib/pgsql/9.1/data/postgresql.conf
    sudo -u postgres bash -c "cat >> /var/lib/pgsql/9.1/data/postgresql.conf" <<EOT
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
  if ! sudo grep --quiet PSQL_HOME /etc/profile; then
    sudo sh -c "echo 'export PSQL_HOME=/usr/pgsql-9.1' >> /etc/profile"
    source /etc/profile
  fi

  # update pg_hba.conf to trust authentication
  if ! sudo grep -i --quiet HOOT /var/lib/pgsql/9.1/data/pg_hba.conf; then
    sudo -u postgres sed -i.bak s/peer/trust/ /var/lib/pgsql/9.1/data/pg_hba.conf
    sudo -u postgres sed -i.bak s/ident/trust/ /var/lib/pgsql/9.1/data/pg_hba.conf
  fi

  sudo service postgresql-9.1 restart

  echo "Installing FileGDB"
  sudo rm -rf ${HOOT_CORE_INSTALL_DIR}/FileGDB_API-1.3
  sudo tar -xzf ${HOOT_HOME_RPMS}/FileGDB_API_1_3-64.tar.gz --directory ${HOOT_CORE_INSTALL_DIR}
  sudo chown -R root:${CORE_INSTALL_USER_GROUP} ${HOOT_CORE_INSTALL_DIR}/FileGDB_API
  sudo cp -R ${HOOT_CORE_INSTALL_DIR}/FileGDB_API ${HOOT_CORE_INSTALL_DIR}/FileGDB_API-1.3
  sudo cp ${HOOT_CORE_INSTALL_DIR}/FileGDB_API-1.3/lib/* ${HOOT_CORE_INSTALL_DIR}/lib/

  echo "Installing GDAL..."
  sudo tar -xzf ${HOOT_HOME_RPMS}/gdal-1.10.1.tar.gz --directory ${HOOT_CORE_INSTALL_DIR}
  cd ${HOOT_CORE_INSTALL_DIR}/gdal-1.10.1
  sudo ./configure -q --with-pg=/usr/pgsql-9.1/bin/pg_config --with-fgdb=/usr/local/FileGDB_API-1.3/ --with-python --prefix=/usr/local 
  
  sudo make -sj$(nproc) && sudo make install

  cd swig/python
  sudo python setup.py build
  sudo python setup.py install

  if ! sudo grep --quiet GDAL_LIB_DIR /etc/profile; then
    sudo sh -c "echo 'export GDAL_LIB_DIR=${HOOT_CORE_INSTALL_DIR}/lib' >> /etc/profile"
    source /etc/profile
  fi
  if ! sudo grep --quiet GDAL_DATA /etc/profile; then
     sudo sh -c "echo 'export GDAL_DATA=${HOOT_CORE_INSTALL_DIR}/share/gdal' >> /etc/profile"
     source /etc/profile
   fi

  echo "Installing Qt..."
  sudo yum -y remove qt*
  sudo rm -f ${HOOT_CORE_INSTALL_DIR}/qt-everywhere-opensource-src-4.8.6.tar.gz
  rm -rf qt-everywhere-opensource-src-4.8.6
  sudo rm -rf ${HOOT_CORE_INSTALL_DIR}/qt-everywhere-opensource-src-4.8.6
  sudo tar -xzf ${HOOT_HOME_RPMS}/qt-everywhere-opensource-src-4.8.6.tar.gz --directory ${HOOT_CORE_INSTALL_DIR}
  sudo chown -R root:${CORE_INSTALL_USER_GROUP} ${HOOT_CORE_INSTALL_DIR}/qt-everywhere-opensource-src-4.8.6

  sudo rpm -ivh ${HOOT_HOME_RPMS}/qt-everywhere-opensource-src-4.8.6-1.x86_64.rpm
  sudo rm -f /usr/bin/qmake
  sudo ln -s /usr/lib64/qt4/bin/qmake /usr/bin/qmake

  if ! sudo grep --quiet QT_HOME /etc/profile; then
    sudo sh -c "echo 'export QT_HOME=${HOOT_CORE_INSTALL_DIR}/qt-everywhere-opensource-src-4.8.6' >> /etc/profile"
    source /etc/profile
  fi

  if ! sudo grep --quiet NODE_PATH /etc/profile; then
    echo "Installing node js dependencies"
    sudo npm config set registry http://registry.npmjs.org/
    sudo npm install -g xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler
    echo 'Adding NODE_PATH to user environment'
    sudo sh -c "echo 'export NODE_PATH=/usr/lib/node_modules' >> /etc/profile"
    source /etc/profile
  fi

  echo "Configuring Tomcat..."
  sudo service tomcat6 start
  if ! sudo grep -i --quiet HOOT /usr/sbin/tomcat6; then
    echo "Configuring tomcat6 environment"
    sudo bash -c "cat >> /usr/sbin/tomcat6" <<EOT
#--------------
# Hoot Settings
#--------------
HOOT_HOME=${HOOT_CORE_INSTALL_DIR}/hoot
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOOT_CORE_INSTALL_DIR}/lib:${HOOT_HOME}/lib:${HOOT_HOME}/pretty-pipes/lib
GDAL_DATA=${HOOT_CORE_INSTALL_DIR}/share/gdal
GDAL_LIB_DIR=${HOOT_CORE_INSTALL_DIR}/lib
HOOT_WORKING_NAME=hoot
QT_HOME=${HOOT_CORE_INSTALL_DIR}/qt-everywhere-opensource-src-4.8.6
PSQL_HOME=/usr/pgsql-9.1
PATH=$HOOT_HOME/bin:${HOOT_CORE_INSTALL_DIR}/lib:${HOOT_CORE_INSTALL_DIR}/bin:${PATH}:${QT_HOME}/bin:${PSQL_HOME}/bin
EOT
  fi
	
  # Create Tomcat context path for tile images
  if ! sudo grep -i --quiet 'ingest/processed' /etc/tomcat6/server.xml; then
    echo "Adding Tomcat context path for tile images"
    sudo sed -i.bak "s@<\/Host>@ <Context docBase=\"\/usr\/local\/hoot\/ingest\/processed\" path=\"\/static\" \/>\n &@" /etc/tomcat6/server.xml
  fi
	
  # Allow linking in Tomcat context
  if ! sudo grep -i --quiet 'allowLinking="true"' /etc/tomcat6/context.xml; then
    echo "Set allowLinking to true in Tomcat context"
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" /etc/tomcat6/context.xml
  fi
	
  # Create directory for webapp
  if [ ! -d /usr/share/tomcat6/.deegree ]; then
	echo "Creating directory for webapp"
	sudo mkdir /usr/share/tomcat6/.deegree
	sudo chown tomcat:tomcat /usr/share/tomcat6/.deegree
  fi
	
  # Set Tomcat to autostart
  sudo /sbin/chkconfig --add tomcat6
  sudo /sbin/chkconfig tomcat6 on
  sudo service tomcat6 restart

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
  sudo service postgresql-9.1 restart
  sleep 10
  	
  # go inside Hoot to build
  cd ${HOOT_HOME}
  source ./SetupEnv.sh
	
  # Create empty ChangeLog file
  touch ChangeLog
	
  echo "Configuring Hoot"
  aclocal && autoconf && autoheader && automake
	
  echo "./configure: ./configure --with-rnd"
  ./configure --with-rnd

  echo "Building Hoot"
  make clean -sj$(nproc)
  make -sj$(nproc)
	
  #Copy docs from github into /usr/local/hoot/docs/ directory
  wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ Algorithms.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.Algorithms.pdf
  wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ Developer\ Guide.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.Developer.Guide.pdf
  wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ Installation\ Instructions.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.Installation.Instructions.pdf
  wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ User\ Guide.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.User.Guide.pdf
  wget -O ${HOOT_HOME}/docs/Hootenanny\ -\ User\ Interface\ Guide.pdf https://github.com/ngageoint/hootenanny/releases/download/v0.2.21/Hootenanny.-.User.Interface.Guide.pdf

  # configure hoot core permissions
  sudo chmod -R 755 ${HOOT_HOME}
  sudo chown -R tomcat:tomcat ${HOOT_HOME}
  sudo chmod -R 755 ${HOOT_CORE_INSTALL_DIR}/hootenanny-${HOOT_VERSION}
  sudo chown -R tomcat:tomcat ${HOOT_CORE_INSTALL_DIR}/hootenanny-${HOOT_VERSION}
  sudo ln -s ${HOOT_CORE_INSTALL_DIR}/hoot/bin/hoot ${HOOT_CORE_INSTALL_DIR}/bin/hoot
	
  hoot version
  sleep 3

  # Deploy to Tomcat
  echo "Deploying Hoot to Tomcat"
  sudo sh -c "echo 'export TOMCAT6_HOME=/var/lib/tomcat6/webapps' >> /etc/profile"
  source /etc/profile
	
  sudo chmod 775 ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}.war
  sudo chown tomcat:tomcat ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}.war

  sudo -u tomcat rm -f ${TOMCAT6_HOME}/hoot-services-*.war
  sudo -u tomcat cp ${HOOT_CORE_INSTALL_DIR}/hootenanny-services-${HOOT_VERSION}.war ${TOMCAT6_HOME}/hoot-services.war
  sudo -u tomcat rm -rf ${TOMCAT6_HOME}/hootenanny-id/
  sudo -u tomcat cp -R ${HOOT_CORE_INSTALL_DIR}/hootenanny-${HOOT_VERSION}/hoot-ui ${TOMCAT6_HOME}/hootenanny-id
  sudo -u tomcat rm -rf ${TOMCAT6_HOME}/hootenanny-id/css/img  
  sudo -u tomcat cp -Rf ${TOMCAT6_HOME}/hootenanny-id/dist/img ${TOMCAT6_HOME}/hootenanny-id/css/
	
  echo "Wait for tomcat to extract the hoot-services.war file..."
  sleep 10
	
  # set web services permissions
  sudo chown -R tomcat:tomcat ${TOMCAT6_HOME}/hoot-services
  sudo chmod -R 775 ${TOMCAT6_HOME}/hoot-services
  # needed by WFS
  sudo chmod -R 777 ${TOMCAT6_HOME}/hoot-services/WEB-INF/workspace
	
  # add folders and permissions for web app
  sudo mkdir -p ${HOOT_HOME}/ingest/processed/
  sudo chmod -R 777 ${HOOT_HOME}/ingest
	
  # Restart Tomcat Server - Added by Mitul
  sudo service tomcat6 restart
  sleep 10
  echo "Tomcat Server restarted"

  # Stop iptables
  sudo service iptables stop

  # Source Hoot Environments - Added by Mitul
  source ${HOOT_HOME}/SetupEnv.sh

  #Start nodejs
  nohup npm start

  echo "Hootenanny install of version ${HOOT_VERSION} has been completed.  Access the command line interface by typing: 'hoot'"
  echo "View the web interface at http://localhost:8080/hootenanny-id/"
