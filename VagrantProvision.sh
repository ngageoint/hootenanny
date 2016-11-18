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

echo "### Setup NTP..."
sudo apt-get -q -y install ntp
sudo service ntp stop
sudo ntpd -gq
sudo service ntp start


if [ ! -f /etc/apt/sources.list.d/pgdg.list ]; then
    echo "### Adding PostgreSQL repository to apt..."
    sudo bash -c "echo 'deb http://apt.postgresql.org/pub/repos/apt/ '$(lsb_release -cs)'-pgdg main' > /etc/apt/sources.list.d/pgdg.list"
    wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
    sudo apt-get -qq -y update >> Ubuntu_upgrade.txt 2>&1
    sudo apt-get -q -y upgrade >> Ubuntu_upgrade.txt 2>&1
fi

echo "### Installing dependencies from repos..."
sudo apt-get -q -y install texinfo g++ libicu-dev libqt4-dev git-core libboost-dev libcppunit-dev libcv-dev libopencv-dev libgdal-dev liblog4cxx10-dev libnewmat10-dev libproj-dev python-dev libjson-spirit-dev automake protobuf-compiler libprotobuf-dev gdb libqt4-sql-psql libgeos++-dev swig lcov tomcat6 openjdk-7-jdk openjdk-7-dbg maven libstxxl-dev nodejs-dev nodejs-legacy doxygen xsltproc asciidoc pgadmin3 curl npm libxerces-c28 libglpk-dev libboost-all-dev source-highlight texlive-lang-arabic texlive-lang-hebrew texlive-lang-cyrillic graphviz w3m python-setuptools python python-pip git ccache libogdi3.2-dev gnuplot python-matplotlib libqt4-sql-sqlite ruby ruby-dev xvfb zlib1g-dev patch x11vnc openssh-server htop unzip postgresql-9.5  postgresql-client-9.5 postgresql-9.5-postgis-scripts postgresql-9.5-postgis-2.3 >> Ubuntu_upgrade.txt 2>&1

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
    echo "export HOOT_HOME=\$HOME/hoot" >> ~/.profile
    echo "export PATH=\$PATH:\$HOOT_HOME/bin" >> ~/.profile
    source ~/.profile
fi

if ! grep --quiet "export JAVA_HOME" ~/.profile; then
    echo "Adding Java home to profile..."
    echo "export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64" >> ~/.profile
    source ~/.profile
fi

if ! grep --quiet "export HADOOP_HOME" ~/.profile; then
    echo "Adding Hadoop home to profile..."
    #echo "export HADOOP_HOME=/usr/local/hadoop" >> ~/.profile
    echo "export HADOOP_HOME=\$HOME/hadoop" >> ~/.profile
    echo "export PATH=\$PATH:\$HADOOP_HOME/bin" >> ~/.profile
    source ~/.profile
fi

if ! grep --quiet "PATH=" ~/.profile; then
    echo "Adding path vars to profile..."
    #echo "export PATH=\$PATH:\$HOME/.gem/ruby/1.9.1/bin:\$HOME/bin:$HOOT_HOME/bin" >> ~/.profile
    echo "export PATH=\$PATH:\$HOME/bin:$HOOT_HOME/bin" >> ~/.profile
    source ~/.profile
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
   #sudo gem install mime-types -v 2.6.2
   gem install mime-types
fi
# gem list --local | grep -q capybara
# if [ $? -eq 1 ]; then
#    #sudo gem install capybara -v 2.5.0
#    gem install capybara
# fi
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
      LATEST_RELEASE="`wget --quiet -O- http://chromedriver.storage.googleapis.com/LATEST_RELEASE`"
      wget --quiet http://chromedriver.storage.googleapis.com/$LATEST_RELEASE/chromedriver_linux64.zip
    fi
    unzip -d $HOME/bin chromedriver_linux64.zip
else
  LATEST_RELEASE="`wget --quiet -O- http://chromedriver.storage.googleapis.com/LATEST_RELEASE`"
  if [[ "$(chromedriver --version)" != "ChromeDriver $(LATEST_RELEASE).*" ]]; then
    echo "### Updating Chromedriver"
    rm $HOME/bin/chromedriver
    rm $HOME/chromedriver_linux64.zip
    wget --quiet http://chromedriver.storage.googleapis.com/$LATEST_RELEASE/chromedriver_linux64.zip
    unzip -o -d $HOME/bin chromedriver_linux64.zip
  fi
fi

sudo apt-get autoremove -y

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

if ! mocha --version &>/dev/null; then
    echo "### Installing mocha for plugins test..."
    sudo npm install --silent -g mocha
    # Clean up after the npm install
    sudo rm -rf $HOME/tmp
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

# Configure Tomcat
if ! grep --quiet TOMCAT6_HOME ~/.profile; then
    echo "### Adding Tomcat to profile..."
    echo "export TOMCAT6_HOME=/var/lib/tomcat6" >> ~/.profile
    source ~/.profile
fi

# Add tomcat6 and vagrant to each others groups so we can get the group write working with nfs
if ! groups vagrant | grep --quiet '\btomcat6\b'; then
    echo "Adding vagrant user to tomcat6 user group..."
    sudo usermod -a -G tomcat6 vagrant
fi
if ! groups tomcat6 | grep --quiet "\bvagrant\b"; then
    echo "Adding tomcat6 user to vagrant user group..."
    sudo usermod -a -G vagrant tomcat6
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
HADOOP_HOME=/home/vagrant/hadoop
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib
GDAL_DATA=/usr/local/share/gdal
GDAL_LIB_DIR=/usr/local/lib
HOOT_WORKING_NAME=hoot
PATH=$HOOT_HOME/bin:$PATH
EOT
fi

# Trying this to try to get rid of errors
sudo mkdir -p /usr/share/tomcat6/server/classes
sudo mkdir -p /usr/share/tomcat6/shared/classes
sudo chown -R tomcat6:tomcat6 /usr/share/tomcat6/server
sudo chown -R tomcat6:tomcat6 /usr/share/tomcat6/shared

# Can change it to 000 to get rid of errors
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
# node-export-server using gdal libs compiled from source (fgdb support)
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

# Clean out tomcat logfile. We restart tomcat after provisioning
sudo service tomcat6 stop
sudo rm /var/log/tomcat6/catalina.out

cd ~
# hoot has only been tested successfully with hadoop 0.20.2, which is not available from public repos,
# so purposefully not installing hoot from the repos.
if ! which hadoop > /dev/null ; then
  echo "Installing Hadoop..."
  if [ ! -f hadoop-0.20.2.tar.gz ]; then
    wget --quiet https://archive.apache.org/dist/hadoop/core/hadoop-0.20.2/hadoop-0.20.2.tar.gz
  fi

  if [ ! -f $HOME/.ssh/id_rsa ]; then
    ssh-keygen -t rsa -N "" -f $HOME/.ssh/id_rsa
    cat ~/.ssh/id_rsa.pub >> $HOME/.ssh/authorized_keys
    ssh-keyscan -H localhost >> $HOME/.ssh/known_hosts
  fi
  chmod 600 $HOME/.ssh/authorized_keys

  #cd /usr/local
  cd ~
  sudo tar -zxf $HOME/hadoop-0.20.2.tar.gz
  sudo chown -R vagrant:vagrant hadoop-0.20.2
  sudo ln -s hadoop-0.20.2 hadoop
  sudo chown -R vagrant:vagrant hadoop
  cd hadoop
  sudo find . -type d -exec chmod a+rwx {} \;
  sudo find . -type f -exec chmod a+rw {} \;
  sudo chmod go-w bin
  cd ~

#TODO: remove these home dir hardcodes
sudo rm -f $HADOOP_HOME/conf/core-site.xml
sudo bash -c "cat >> /home/vagrant/hadoop/conf/core-site.xml" <<EOT

<configuration>
  <property>
    <name>fs.default.name</name>
    <value>hdfs://localhost:9000/</value>
  </property>
</configuration>
EOT
sudo rm -f $HADOOP_HOME/conf/mapred-site.xml
sudo bash -c "cat >> /home/vagrant/hadoop/conf/mapred-site.xml" <<EOT

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
sudo rm -f $HADOOP_HOME/conf/hdfs-site.xml
sudo bash -c "cat >> /home/vagrant/hadoop/conf/hdfs-site.xml" <<EOT

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
    <value>/home/vagrant/hadoop/dfs/namesecondary</value>
  </property>
  <property>
    <name>dfs.name.dir</name>
    <value>/home/vagrant/hadoop/dfs/name</value>
  </property>
  <property>
    <name>dfs.data.dir</name>
    <value>/home/vagrant/hadoop/dfs/data</value>
  </property>
</configuration>
EOT

  sudo sed -i.bak 's/# export JAVA_HOME=\/usr\/lib\/j2sdk1.5-sun/export JAVA_HOME=\/usr\/lib\/jvm\/java-1.7.0-openjdk-amd64/g' $HADOOP_HOME/conf/hadoop-env.sh
  sudo sed -i.bak 's/#include <pthread.h>/#include <pthread.h>\n#include <unistd.h>/g' $HADOOP_HOME/src/c++/pipes/impl/HadoopPipes.cc

  sudo mkdir -p $HOME/hadoop/dfs/name/current
  # this could perhaps be more strict
  sudo chmod -R 777 $HOME/hadoop
  echo 'Y' | hadoop namenode -format

  cd /lib
  sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
  cd /lib64
  sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
  cd ~

  echo '1' | sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-1.7.0-openjdk-amd64/bin/java 1
  echo '1' | sudo update-alternatives --config java
  echo '1' | sudo update-alternatives --install "/usr/bin/javac" "javac" "$JAVA_HOME/bin/javac" 1
  echo '1' | sudo update-alternatives --config javac

  # test hadoop out
  #stop-all.sh
  #start-all.sh
  #hadoop fs -ls /
  #hadoop jar ./hadoop-0.20.2-examples.jar pi 2 100
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
