#!/usr/bin/env bash

HOOT_HOME=$HOME/hoot
echo HOOT_HOME: $HOOT_HOME
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
sudo apt-get -q -y install ntp 2>&1
sudo service ntp stop
sudo ntpd -gq
sudo service ntp start

if ! java -version 2>&1 | grep --quiet 1.8.0_112; then
    echo "### Installing Java 8..."

    # jdk-8u112-linux-x64.tar.gz's official checksums:
    #    sha256: 777bd7d5268408a5a94f5e366c2e43e720c6ce4fe8c59d9a71e2961e50d774a5
    #    md5: de9b7a90f0f5a13cfcaa3b01451d0337
    echo "de9b7a90f0f5a13cfcaa3b01451d0337  /tmp/jdk-8u112-linux-x64.tar.gz" > /tmp/jdk.md5

    if [ ! -f /tmp/jdk-8u112-linux-x64.tar.gz ] || ! md5sum -c /tmp/jdk.md5; then
        echo "Downloading jdk-8u112-linux-x64.tar.gz ...."
        sudo wget --quiet --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" http://download.oracle.com/otn-pub/java/jdk/8u112-b15/jdk-8u112-linux-x64.tar.gz -P /tmp
        echo "Finished download of jdk-8u112-linux-x64.tar.gz"
    fi

    sudo tar -xvzf /tmp/jdk-8u112-linux-x64.tar.gz --directory=/tmp >/dev/null

    if [[ ! -e /usr/lib/jvm ]]; then
        sudo mkdir /usr/lib/jvm
    else
        if [[ -e /usr/lib/jvm/oracle_jdk8 ]]; then
            sudo rm -rf /usr/lib/jvm/oracle_jdk8
        fi
    fi

    sudo mv -f /tmp/jdk1.8.0_112 /usr/lib/jvm/oracle_jdk8
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

echo "### Installing dependencies from repos..."
sudo apt-get -q -y install texinfo g++ libicu-dev libqt4-dev git-core libboost-dev libcppunit-dev \
 libcv-dev libopencv-dev liblog4cxx10-dev libnewmat10-dev libproj-dev python-dev libjson-spirit-dev \
 automake protobuf-compiler libprotobuf-dev gdb libqt4-sql-psql libgeos++-dev swig lcov maven \
 libstxxl-dev nodejs-dev nodejs-legacy doxygen xsltproc asciidoc curl npm libxerces-c28 \
 libglpk-dev libboost-all-dev source-highlight texlive-lang-arabic texlive-lang-hebrew \
 w3m texlive-lang-cyrillic graphviz python-setuptools python python-pip git ccache distcc libogdi3.2-dev \
 gnuplot python-matplotlib libqt4-sql-sqlite ruby ruby-dev xvfb zlib1g-dev patch x11vnc openssh-server \
 htop unzip postgresql-9.5 postgresql-client-9.5 postgresql-9.5-postgis-scripts postgresql-9.5-postgis-2.3 \
 libpango-1.0-0 libappindicator1 >> Ubuntu_upgrade.txt 2>&1

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

# Configure https alternative mirror for maven isntall, this can likely be removed once
# we are using maven 3.2.3 or higher
sudo /usr/bin/perl $HOOT_HOME/scripts/maven/SetMavenHttps.pl

if ! grep --quiet "export HOOT_HOME" ~/.profile; then
    echo "Adding hoot home to profile..."
    echo "export HOOT_HOME=\$HOME/hoot" >> ~/.profile
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

if ! grep --quiet "export HADOOP_HOME" ~/.profile; then
    echo "Adding Hadoop home to profile..."
    echo "export HADOOP_HOME=\$HOME/hadoop" >> ~/.profile
    echo "export PATH=\$PATH:\$HADOOP_HOME/bin" >> ~/.profile
    source ~/.profile
fi

if ! grep --quiet "PATH=" ~/.profile; then
    echo "Adding path vars to profile..."
    echo "export PATH=\$PATH:\$JAVA_HOME/bin:\$HOME/bin:$HOOT_HOME/bin" >> ~/.profile
    source ~/.profile
fi

# Whether the client uses distcc or not, have distcc set up and ready to go.  To turn it on, 
# enable it in LocalConfig.pri, configure the slaves in ~/.distcc/hosts, and launch distccd on 
# the slaves.
if [ ! -f ~/.distcc/hosts ]; then
    echo "Adding distcc hosts file..."
    mkdir -p ~/.distcc
    echo "localhost/4" >> ~/.distcc/hosts
fi
if ! grep --quiet "DISTCC_TCP_CORK=0" ~/.profile; then
    echo "Configuring distcc in profile..."
    echo "export DISTCC_TCP_CORK=0" >> ~/.profile
    source ~/.profile
fi

if ! ruby -v | grep --quiet 2.3.0; then
    # Ruby via rvm - from rvm.io
    gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3 2>&1

    curl -sSL https://raw.githubusercontent.com/rvm/rvm/master/binscripts/rvm-installer | bash -s stable

    source /home/vagrant/.rvm/scripts/rvm

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

if  ! dpkg -l | grep --quiet google-chrome-stable; then
    echo "### Installing Chrome..."
    if [ ! -f google-chrome-stable_current_amd64.deb ]; then
      wget --quiet https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
    fi
    sudo apt-get -f -y -q install
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
  if [[ "$(chromedriver --version)" != "ChromeDriver $LATEST_RELEASE."* ]]; then
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


# For convenience, set the version of GDAL to download and install
GDAL_VERSION=2.1.3

if ! $( hash ogrinfo >/dev/null 2>&1 && ogrinfo --version | grep -q $GDAL_VERSION && ogrinfo --formats | grep -q FileGDB ); then
    if [ ! -f gdal-$GDAL_VERSION.tar.gz ]; then
        echo "### Downloading GDAL $GDAL_VERSION source..."
        wget --quiet http://download.osgeo.org/gdal/$GDAL_VERSION/gdal-$GDAL_VERSION.tar.gz
    fi
    if [ ! -d gdal-$GDAL_VERSION ]; then
        echo "### Extracting GDAL $GDAL_VERSION source..."
        tar zxfp gdal-$GDAL_VERSION.tar.gz
    fi

    if [ ! -f FileGDB_API_1_5_64.tar.gz ]; then
        echo "### Downloading FileGDB API source..."
        wget --quiet https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_1.5/FileGDB_API_1_5_64.tar.gz
    fi
    if [ ! -d /usr/local/FileGDB_API ]; then
        echo "### Extracting FileGDB API source & installing lib..."
        sudo mkdir -p /usr/local/FileGDB_API && sudo tar xfp FileGDB_API_1_5_64.tar.gz --directory /usr/local/FileGDB_API --strip-components 1
        sudo sh -c "echo '/usr/local/FileGDB_API/lib' > /etc/ld.so.conf.d/filegdb.conf"
    fi

    echo "### Building GDAL $GDAL_VERSION w/ FileGDB..."
    export PATH=/usr/local/lib:/usr/local/bin:$PATH
    cd gdal-$GDAL_VERSION
    touch config.rpath
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
sudo $HOOT_HOME/scripts/tomcat/tomcat8/ubuntu/tomcat8_install.sh

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
# hoot has only been tested successfully with hadoop 0.20.2, which is not available from public repos,
# so purposefully not installing hoot from the repos.
if ! hash hadoop >/dev/null 2>&1 ; then
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

  sudo sed -i.bak 's/# export JAVA_HOME=\/usr\/lib\/j2sdk1.5-sun/export JAVA_HOME=\/usr\/lib\/jvm\/oracle_jdk8/g' $HADOOP_HOME/conf/hadoop-env.sh
  sudo sed -i.bak 's/#include <pthread.h>/#include <pthread.h>\n#include <unistd.h>/g' $HADOOP_HOME/src/c++/pipes/impl/HadoopPipes.cc

  sudo mkdir -p $HOME/hadoop/dfs/name/current
  # this could perhaps be more strict
  sudo chmod -R 777 $HOME/hadoop
  sudo chmod go-w $HOME/hadoop/bin $HOME/hadoop
  echo 'Y' | hadoop namenode -format

  cd /lib
  sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
  cd /lib64
  sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
  cd ~

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

# Update marker file date now that dependency and config stuff has run
# The make command will exit and provide a warning to run 'vagrant provision'
# if the marker file is older than this file (VagrantProvision.sh)
touch Vagrant.marker
# Now we are ready to build Hoot.  The VagrantBuild.sh script will build Hoot.

# switch to auto mode and use the highest priority installed alternatives for Java.
sudo update-alternatives --auto java
sudo update-alternatives --auto javac


if [ ! -d "$HOOT_HOME/userfiles/ingest/processed" ]; then
    mkdir -p $HOOT_HOME/userfiles/ingest/processed
fi

# wipe out all dirs. tmp and upload now reside under $HOOT_HOME/userfiles/
rm -rf $HOOT_HOME/upload
rm -rf $HOOT_HOME/tmp

if [ -d "$HOOT_HOME/data/reports" ]; then
    echo "Moving contents of $HOOT_HOME/data/reports to $HOOT_HOME/userfiles/"
    cp -R $HOOT_HOME/data/reports $HOOT_HOME/userfiles/
    rm -rf $HOOT_HOME/data/reports
fi

if [ -d "$HOOT_HOME/customscript" ]; then
    echo "Moving contents of $HOOT_HOME/customscript to $HOOT_HOME/userfiles/"
    cp -R $HOOT_HOME/customscript $HOOT_HOME/userfiles/
    rm -rf $HOOT_HOME/customscript
fi

if [ -d "$HOOT_HOME/ingest" ]; then
    echo "Moving contents of $HOOT_HOME/ingest to $HOOT_HOME/userfiles/"
    cp -R $HOOT_HOME/ingest $HOOT_HOME/userfiles/
    rm -rf $HOOT_HOME/ingest
fi

# Always start with a clean $HOOT_HOME/userfiles/tmp
rm -rf $HOOT_HOME/userfiles/tmp
