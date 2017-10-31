#!/usr/bin/env bash
set -e

# Don't install Hadoop if requested.
if [ "${WITH_HADOOP}" != "yes" ]; then
    exit 0
fi

HADOOP_TAR=hadoop-$HADOOP_VERSION.tar.gz
HADOOP_URL=$HADOOP_BASE_URL/hadoop-$HADOOP_VERSION/$HADOOP_TAR

cd ~

# hoot has only been tested successfully with hadoop 0.20.2, which is not available from public repos,
# so purposefully not installing hoot from the repos.
if ! hash hadoop >/dev/null 2>&1 ; then
  echo "Installing Hadoop..."
  if [ ! -f ~/$HADOOP_TAR ]; then
    wget --quiet $HADOOP_URL
  fi

  if [ ! -f ~/.ssh/id_rsa ]; then
    ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
    cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
    ssh-keyscan -H localhost >> ~/.ssh/known_hosts
  fi
  chmod 0600 ~/.ssh/authorized_keys

  cd ~
  sudo tar -zxf ~/$HADOOP_TAR
  sudo chown -R $VMUSER:$VMGROUP hadoop-0.20.2
  sudo ln -s hadoop-0.20.2 hadoop
  sudo chown -R $VMUSER:$VMGROUP hadoop
  cd hadoop
  sudo find . -type d -exec chmod a+rwx {} \;
  sudo find . -type f -exec chmod a+rw {} \;
  cd ~

sudo rm -f $HADOOP_HOME/conf/core-site.xml
sudo bash -c "cat >> $HADOOP_HOME/conf/core-site.xml" <<EOT

<configuration>
  <property>
    <name>fs.default.name</name>
    <value>hdfs://localhost:9000/</value>
  </property>
</configuration>
EOT
sudo rm -f $HADOOP_HOME/conf/mapred-site.xml
sudo bash -c "cat >> $HADOOP_HOME/conf/mapred-site.xml" <<EOT

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
sudo bash -c "cat >> $HADOOP_HOME/conf/hdfs-site.xml" <<EOT

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
    <value>$HADOOP_HOME/dfs/namesecondary</value>
  </property>
  <property>
    <name>dfs.name.dir</name>
    <value>$HADOOP_HOME/dfs/name</value>
  </property>
  <property>
    <name>dfs.data.dir</name>
    <value>$HADOOP_HOME/dfs/data</value>
  </property>
</configuration>
EOT

  sudo sed -i.bak "s/# export JAVA_HOME=\/usr\/lib\/j2sdk1.5-sun/export JAVA_HOME=${JAVA_HOME//\//\\/}/g" $HADOOP_HOME/conf/hadoop-env.sh
  sudo sed -i.bak 's/#include <pthread.h>/#include <pthread.h>\n#include <unistd.h>/g' $HADOOP_HOME/src/c++/pipes/impl/HadoopPipes.cc

  sudo mkdir -p $HADOOP_HOME/dfs/name/current
  # this could perhaps be more strict
  sudo chmod -R 777 $HADOOP_HOME
  sudo chmod go-w $HADOOP_HOME/bin $HADOOP_HOME
  echo 'Y' | hadoop namenode -format

  cd /lib
  sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
  cd /lib64
  sudo ln -s $JAVA_HOME/jre/lib/amd64/server/libjvm.so libjvm.so
  cd ~
fi
