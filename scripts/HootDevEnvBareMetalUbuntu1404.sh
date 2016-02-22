#!/usr/bin/env bash

#similar to the hoot vagrant setup, except not virtualized and with added developer tools; not actively supported and not guaranteed to be in sync with the latest hoot...use this as a reference ONLY as one possible way to set up a hoot developer environment
#see the vagrant install for the latest and greatest hoot setup; USE THIS AT YOUR OWN RISK

cd ~
mkdir -p hoot

if ! grep -i --quiet HOOT $HOME/.profile; then
sudo bash -c "cat >> $HOME/.profile" <<EOT

export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64
export HADOOP_HOME=/usr/local/hadoop
export HOOT_HOME=/home/bwitham/hoot
export PATH=/usr/local/bin:/usr/local/lib:$PATH:/usr/lib/x86_64-linux-gnu:$HADOOP_HOME/bin:$HOOT_HOME/bin
EOT
fi
source $HOME/.profile

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
# use tomcat 7 here instead of tomcat 6 (what's tested with hoot), since eclipse installed from repos (installed in this script later) needs tomcat 7 (eclipse tomcat 6 install is a little cumbersome)
echo "Installing dependencies"
sudo apt-get install -y texinfo g++ libicu-dev libqt4-dev git-core libboost-dev libcppunit-dev libcv-dev libopencv-dev libgdal-dev liblog4cxx10-dev libnewmat10-dev libproj-dev python-dev libjson-spirit-dev automake1.11 protobuf-compiler libprotobuf-dev gdb libqt4-sql-psql libgeos++-dev swig lcov tomcat7 openjdk-7-jdk openjdk-7-dbg maven libstxxl-dev nodejs-dev nodejs-legacy doxygen xsltproc asciidoc pgadmin3 curl npm libxerces-c28 libglpk-dev libboost-all-dev source-highlight texlive-lang-arabic texlive-lang-hebrew texlive-lang-cyrillic graphviz w3m python-setuptools python python-pip git ccache libogdi3.2-dev gnuplot python-matplotlib libqt4-sql-sqlite wamerican-insane

if ! dpkg -l | grep --quiet wamerican-insane; then
    # See /usr/share/doc/dictionaries-common/README.problems for details
    # http://www.linuxquestions.org/questions/debian-26/dpkg-error-processing-dictionaries-common-4175451951/
    sudo apt-get install -y wamerican-insane
    sudo /usr/share/debconf/fix_db.pl
    sudo dpkg-reconfigure dictionaries-common
fi

# don't think ui unit tests are running with hoot outside of the nightly tests, so skipping this for now
# Install deps for Cucumber tests
#sudo apt-get install ruby ruby-dev xvfb
#sudo gem install mime-types -v 2.6.2
#sudo gem install capybara -v 2.5.0
#sudo gem install cucumber capybara-webkit selenium-webdriver rspec capybara-screenshot

#sudo apt-get autoremove -y

# Install Chrome browser for Cucumber
#if [ ! -f google-chrome-stable_current_amd64.deb ]; then
#    echo "Installing Chrome"
#    wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
#    sudo dpkg -i google-chrome-stable_current_amd64.deb
#    sudo apt-get -f install -y
#fi

# Install Chromedriver
if [ ! -f bin/chromedriver ]; then
    echo "Installing Chromedriver"
    mkdir -p /home/bwitham/bin
    wget http://chromedriver.storage.googleapis.com/2.20/chromedriver_linux64.zip
    unzip -d /home/bwitham/bin chromedriver_linux64.zip
fi

# Hoot Baseline is PostgreSQL 9.1 and PostGIS 1.5, so we need a deb file and
# then remove 9.4
if [ ! -f postgresql-9.1-postgis_1.5.3-2_amd64.deb ]; then
    echo "Installing PostgreSQL 9.1"
    wget http://launchpadlibrarian.net/86690107/postgresql-9.1-postgis_1.5.3-2_amd64.deb
    sudo dpkg -i postgresql-9.1-postgis_1.5.3-2_amd64.deb
    sudo apt-get -f install -y
    # fixes missing dependency of postgis 1.5 by installing postgresql 9.1. 9.1 is installed listening on the default port, 5432. It unfortunately also installs postgres 9.4 but we remove that cleanly in the following steps, while leaving postgres 9.1 untouched
    echo "Removing PostgreSQL 9.4"
    sudo apt-get purge -y postgresql-9.4 postgresql-client-9.4 postgresql-9.4-postgis-scripts postgresql-9.5 postgresql-client-9.5 postgresql-9.5-postgis-scripts
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
    sudo make -j$(nproc)
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

# Module needed for OSM API db test
if [ ! -d /home/bwitham/.cpan ]; then
    (echo y;echo o conf prerequisites_policy follow;echo o conf commit)|sudo cpan
    sudo perl -MCPAN -e 'install XML::Simple'
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

POSTGRES_CONFIG_CHANGED=false
# Update shared memory limits in OS
if ! sysctl -e kernel.shmmax | grep --quiet 1173741824; then
    echo "Setting kernel.shmmax"
    sudo sysctl -w kernel.shmmax=1173741824
    sudo sh -c "echo 'kernel.shmmax=1173741824' >> /etc/sysctl.conf"
    POSTGRES_CONFIG_CHANGED=true
fi
if ! sysctl -e kernel.shmall | grep --quiet 2097152; then
    echo "Setting kernel.shmall"
    sudo sysctl -w kernel.shmall=2097152
    sudo sh -c "echo 'kernel.shmall=2097152' >> /etc/sysctl.conf"
    POSTGRES_CONFIG_CHANGED=true
fi
if [ "$POSTGRES_CONFIG_CHANGE" == "true" ]; then
  sudo service postgresql restart
fi

# Configure Tomcat
if ! grep -i --quiet HOOT /etc/default/tomcat7; then
echo "Configuring tomcat environment"
sudo bash -c "cat >> /etc/default/tomcat7" <<EOT

#--------------
# Hoot Settings
#--------------
HOOT_HOME=/home/bwitham/hoot
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib
GDAL_DATA=/usr/local/share/gdal
GDAL_LIB_DIR=/usr/local/lib
HOOT_WORKING_NAME=hoot
PATH=$HOOT_HOME/bin:$PATH
EOT
fi

# Change Tomcat umask to group write
#if ! grep -i --quiet 'umask 002' /etc/default/tomcat6; then
#echo "Changing Tomcat umask to group write"
#sudo bash -c "cat >> /etc/default/tomcat6" <<EOT
## Set tomcat6 umask to group write because all files in shared folder are owned by vagrant
#umask 002
#EOT
#fi

# Change Tomcat java opts
if grep -i --quiet '^JAVA_OPTS=.*\-Xmx128m' /etc/default/tomcat7; then
    echo "Changing Tomcat java opts"
    sudo sed -i.bak "s@\-Xmx128m@\-Xms512m \-Xmx2048m \-XX:PermSize=512m \-XX:MaxPermSize=4096m@" /etc/default/tomcat7
fi

# Fix env var path for GDAL_DATA
if grep -i --quiet 'gdal/1.10' /etc/default/tomcat7; then
    echo "Fixing Tomcat GDAL_DATA env var path"
    sudo sed -i.bak s@^GDAL_DATA=.*@GDAL_DATA=\/usr\/local\/share\/gdal@ /etc/default/tomcat7
fi

# Remove gdal libs installed by libgdal-dev that interfere with
# renderdb-export-server using gdal libs compiled from source (fgdb support)
if [ -f /usr/lib/libgdal.* ]; then
    echo "Removing GDAL libs installed by libgdal-dev"
    sudo rm /usr/lib/libgdal.*
fi

# Create Tomcat context path for tile images
if ! grep -i --quiet 'ingest/processed' /etc/tomcat7/server.xml; then
    echo "Adding Tomcat context path for tile images"
    sudo sed -i.bak "s@<\/Host>@  <Context docBase=\"\/home\/bwitham\/hoot\/ingest\/processed\" path=\"\/static\" \/>\n      &@" /etc/tomcat7/server.xml
fi

# Allow linking in Tomcat context
if ! grep -i --quiet 'allowLinking="true"' /etc/tomcat7/context.xml; then
    echo "Set allowLinking to true in Tomcat context"
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" /etc/tomcat7/context.xml
fi

# Create directory for webapp
if [ ! -d /usr/share/tomcat7/.deegree ]; then
    echo "Creating directory for webapp"
    sudo mkdir /usr/share/tomcat7/.deegree
    sudo chown tomcat7:tomcat7 /usr/share/tomcat7/.deegree
fi

#hoot has only been tested with hadoop 0.20.2 for the most part, so not installing from repos
if [ ! -f hadoop-0.20.2.tar.gz ]; then
  wget https://archive.apache.org/dist/hadoop/core/hadoop-0.20.2/hadoop-0.20.2.tar.gz
  cd /usr/local
  sudo tar -zxvf $HOME/hadoop-0.20.2.tar.gz
  sudo ln -s hadoop-0.20.2 hadoop
  cd hadoop
  sudo find . -type d -exec chmod a+rwx {} \;
  sudo find . -type f -exec chmod a+rw {} \;

  sudo rm -f /usr/local/hadoop/conf/core-site.xml
sudo bash -c "cat >> /usr/local/hadoop/conf/core-site.xml" <<EOT

<configuration>
  <property>
    <name>fs.default.name</name>
    <value>hdfs://localhost:9000/</value>
  </property>
</configuration>
EOT
sudo rm -f /usr/local/hadoop/conf/mapred-site.xml
sudo bash -c "cat >> /usr/local/hadoop/conf/mapred-site.xml" <<EOT

<configuration>
  <property>
    <name>mapred.job.tracker</name>
    <value>localhost:9001</value>
  </property>
  <property>
    <name>mapred.job.tracker.http.address</name>
    <value>0.0.0.0:50030</value>
  </property>
  <property>
    <name>mapred.task.tracker.http.address</name>
    <value>0.0.0.0:50060</value>
  </property>
  <property>
    <name>mapred.child.java.opts</name>
    <value>-Xmx2048m</value>
  </property>
  <property>
    <name>mapred.map.tasks</name>
    <value>17</value>
  </property>
  <property>
    <name>mapred.tasktracker.map.tasks.maximum</name>
    <value>4</value>
  </property>
  <property>
    <name>mapred.tasktracker.reduce.tasks.maximum</name>
    <value>2</value>
  </property>
  <property>
    <name>mapred.reduce.tasks</name>
    <value>1</value>
  </property>
</configuration>
EOT
sudo rm -f /usr/local/hadoop/conf/hdfs-site.xml
sudo bash -c "cat >> /usr/local/hadoop/conf/hdfs-site.xml" <<EOT

<configuration>
  <property>
    <name>dfs.secondary.http.address</name>
    <value>0.0.0.0:50090</value>
  </property>
  <property>
    <name>dfs.datanode.address</name>
    <value>0.0.0.0:50010</value>
  </property>
  <property>
    <name>dfs.datanode.http.address</name>
    <value>0.0.0.0:50075</value>
  </property>
  <property>
    <name>dfs.datanode.ipc.address</name>
    <value>0.0.0.0:50020</value>
  </property>
  <property>
    <name>dfs.http.address</name>
    <value>0.0.0.0:50070</value>
  </property>
  <property>
    <name>dfs.datanode.https.address</name>
    <value>0.0.0.0:50475</value>
  </property>
  <property>
    <name>dfs.https.address</name>
    <value>0.0.0.0:50470</value>
  </property>
  <property>
    <name>dfs.replication</name>
    <value>2</value>
  </property>
  <property>
    <name>dfs.umaskmode</name>
    <value>002</value>
  </property>
  <property>
    <name>fs.checkpoint.dir</name>
    <value>/hadoop/dfs/namesecondary</value>
  </property>
  <property>
    <name>dfs.name.dir</name>
    <value>/hadoop/dfs/name</value>
  </property>
  <property>
    <name>dfs.data.dir</name>
    <value>/hadoop/dfs/data</value>
  </property>
</configuration>
EOT
   sudo sed -i.bak 's/# export JAVA_HOME=\/usr\/lib\/j2sdk1.5-sun/export JAVA_HOME=\/usr\/lib\/jvm\/java-1.7.0-openjdk-amd64/g' /usr/local/hadoop/conf/hadoop-env.sh
  sudo sed -i.bak 's/#include <pthread.h>/#include <pthread.h>\n#include <unistd.h>/g' /usr/local/hadoop/src/c++/pipes/impl/HadoopPipes.cc
  if [ ! -d /hadoop ]; then
    sudo mkdir -p /hadoop/dfs/name/current
    sudo chmod -R 777 /hadoop
    hadoop namenode -format 
    #prompt here not working
    #yes Y #(make sure to press CAPITAL Y for yes -- lower case y causes a failure)
  fi

  cd /lib
  sudo ln -s /usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server/libjvm.so libjvm.so
  cd /lib64
  sudo ln -s /usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server/libjvm.so libjvm.so
  #TODO: don't know how to automate these yet...will need to press "1" and enter at the prompt
  sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-1.7.0-openjdk-amd64/bin/java 1
  sudo update-alternatives --config java
  sudo update-alternatives --install "/usr/bin/javac" "javac" "/usr/lib/jvm/java-1.7.0-openjdk-amd64/bin/javac" 1
  sudo update-alternatives --config javac

  start-all.sh
  hadoop fs -ls /
  cd $HADOOP_HOME
  hadoop jar ./hadoop-0.20.2-examples.jar pi 2 100
fi

# developer tools extra setup

# this service will conflict with running tomcat in eclipse
sudo service tomcat7 stop
# eclipse install here overwrites the existing standard tested tomcat 6 install; this environment assumes you'll be doing your tomcat debugging from eclipse...so not a big deal
cd $HOME
sudo apt-get install meld qtcreator distcc htop synergy gparted eclipse eclipse-jdt eclipse-pde eclipse-platform* libtomcat7-java
sudo apt-get remove qt5-default 
sudo apt-get autoremove

if ! grep -i --quiet '-Xmx' /etc/default/eclipse.ini; then
  echo "Configuring Eclipse..."
sudo bash -c "cat >> /etc/default/eclipse.ini" <<EOT

-Dosgi.requiredJavaVersion=1.7
-XX:MaxPermSize=2048m
-Xms512m
-Xmx2048m
EOT
fi
if ! grep -i --quiet '--launcher.XXMaxPerSize' /etc/default/eclipse.ini; then
  echo "Configuring Eclipse..."
sudo bash -c "cat >> /etc/default/eclipse.ini" <<EOT

--launcher.XXMaxPermSize
2048m
EOT
fi

cd $HOME
if [ ! -d $HOOT_HOME/hoot-core ]; then
  git clone https://github.com/ngageoint/hootenanny.git hoot
fi
cd hoot
source ./SetupEnv.sh

# Tweak dev environment to make tests run faster
if [ ! -f $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf ]; then
    echo 'testJobStatusPollerTimeout=250' > $HOOT_HOME/hoot-services/src/main/resources/conf/local.conf
fi

# end developer tools extra setup

# Update the init.d script for node-mapnik-server
if [ ! -d /etc/init.d/node-mapnik-server ]; then
  sudo cp node-mapnik-server/init.d/node-mapnik-server /etc/init.d
  sudo chmod a+x /etc/init.d/node-mapnik-server
  # Make sure all npm modules are installed
  cd node-mapnik-server
  sudo npm install
  cd ..
fi

# Check that hoot-ui submodule has been init'd and updated
if [ ! "$(ls -A hoot-ui)" ]; then
    echo "hoot-ui is empty"
    echo "init'ing and updating submodule"
    git submodule init && git submodule update
fi

# Build Hoot
echo "Configuring Hoot"
aclocal && autoconf && autoheader && automake && ./configure -q --with-rnd --with-services
if [ ! -f LocalConfig.pri ] && ! grep --quiet QMAKE_CXX LocalConfig.pri; then
    echo 'Customizing LocalConfig.pri'
    cp LocalConfig.pri.orig LocalConfig.pri
    echo 'QMAKE_CXX=ccache g++' >> LocalConfig.pri
fi
echo "Building Hoot"
make clean -sj$(nproc)
make -sj$(nproc)
make docs -sj$(nproc)

