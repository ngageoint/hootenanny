#!/usr/bin/env bash

# This script:  
#   - is meant for installing hoot in a development enviroment to bare metal for performance reasons (faster compiles, etc.)
#   - calls into the vagrant ubuntu script for the first part of the installation
#   - has added developer tools and other goodies
#   - lets you run hoot on hadoop (this part *could* be moved to the vagrant install, if desired) - NOTE: The hadoop step unfortunately isn't fully automated yet...
# you'll have to interact with the command prompt when it runs.
#
# Use this as one possible way to set up a Hootenanny developer environment.  Feel free to tweak your own version of it.
#
# To run this script:
#
#  cd ~
#  git clone git@github.com:ngageoint/hootenanny.git hoot
#  cd hoot
#  git submodule init && git submodule update
#  hoot/scripts/InstallHootDevelopmentEnvironment.sh
#
# Some of the tools installed (Eclipse, etc.) will require additional manual configuration.
#
# TODO: Fix hardcoded HOOT_HOME in VagrantProvision.sh to make this script work.

cd ~

echo "Installing base dependencies..."
hoot/scripts/VagrantProvision.sh

# Use tomcat 7 here instead of tomcat 6, since eclipse installed from repos needs tomcat 7.  I don't like changing the Tomcat version for the development environment, but the impact is minimal, and the eclipse installation can't be automated very easily if this isn't done.

echo "Backing up Tomcat 6 settings..."
mkdir -p ~/tmp
cp /etc/default/tomcat6 ~/tmp
cp /etc/tomcat6/server.xml ~/tmp
cp /etc/tomcat6/context.xml ~/tmp

echo "Removing Tomcat 6..."
sudo apt-get remove tomcat6
sudo apt-get autoremove -y

echo "Installing Tomcat 7..."
sudo apt-get install -y libtomcat7-java

echo "Restoring settings to Tomcat 7..."
sudo cp ~/tmp/tomcat6 /etc/default
sudo cp ~/tmp/server.xml /etc/tomcat7
sudo cp ~/tmp/context.xml /etc/tomcat7
# Do tomcat7 group permissions need to be set on these files in a developer environment?; Do sym links have to be made for the log dirs, etc?

echo "Installing development environment dependencies..."

sudo apt-add-repository ppa:ubuntugis/ppa
sudo apt-get update

# IDE's
sudo apt-get install -y qtcreator eclipse eclipse-jdt eclipse-pde eclipse-platform*

# misc goodies - distcc requires further configuration
sudo apt-get install -y meld distcc htop synergy gparted osmosis qgis

# needed for webex
sudo apt-get install -y libpangoxft-1.0-0:i386 libxv1:i386 libpangox-1.0-0:i386 libgtk2.0-0 libxtst6 libxi-dev libasound2 libgtk2.0-0:i386 libxtst6:i386 libasound2:i386 libgcj14-awt:i386 icedtea-plugin

# If you're fed up with the unity desktop, then uncomment this for xfce; use 'xfce4-display-settings -m' after installing for display config
# sudo apt-get install xfce4

sudo apt-get autoremove -y

#hoot has only been tested with hadoop 0.20.2, so not installing the latest from repos
if ! grep --quiet "export HADOOP_HOME" ~/.profile; then
    echo "Adding hadoop home to profile..."
    echo "export HADOOP_HOME=/usr/local/hadoop" >> ~/.profile
    source ~/.profile
fi
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
    #TODO: prompt here not working
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
  # test it out
  hadoop jar ./hadoop-0.20.2-examples.jar pi 2 100
fi

# Eclipse configuration - You will still additional Eclipse configuration (web server integration, extensions, etc.)  See Developer's Guide for details.
cd $HOOT_HOME
make eclipse  # generate eclipse project files
cd ~
if ! grep -i --quiet '-Xmx' /etc/eclipse.ini; then
  echo "Configuring Eclipse..."
sudo bash -c "cat >> /etc/eclipse.ini" <<EOT

-Dosgi.requiredJavaVersion=1.7
-XX:MaxPermSize=2048m
-Xms512m
-Xmx2048m
EOT
fi
if ! grep -i --quiet '--launcher.XXMaxPerSize' /etc/eclipse.ini; then
  echo "Configuring Eclipse..."
sudo bash -c "cat >> /etc/eclipse.ini" <<EOT

--launcher.XXMaxPermSize
2048m
EOT
fi

# Install vagrant/vbox here so that you can quickly spin up a different configurations of hoot if 
# needed, or troubleshoot the hoot vagrant install itself.
if [ ! -f vagrant_1.8.1_x86_64.deb ]; then
  wget https://releases.hashicorp.com/vagrant/1.8.1/vagrant_1.8.1_x86_64.deb
  sudo dpkg -i vagrant_1.8.1_x86_64.deb
  sudo apt-get -f install -y
fi
if [ ! -f virtualbox-5.0_5.0.14-105127~Ubuntu~trusty_amd64.deb ]; then
  wget http://download.virtualbox.org/virtualbox/5.0.14/virtualbox-5.0_5.0.14-105127~Ubuntu~trusty_amd64.deb
  sudo dpkg -i virtualbox-5.0_5.0.14-105127~Ubuntu~trusty_amd64.deb
  # Need to understand why this needs to be done...
  sudo rm -f /etc/apt/sources.list.d/virtualbox.list
  sudo apt-get -f install -y
fi

