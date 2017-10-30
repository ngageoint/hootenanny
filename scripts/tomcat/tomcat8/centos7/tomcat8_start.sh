#!/usr/bin/env bash

# Wrapper script to start tomcat8
export HOOT_HOME=/home/vagrant/hoot
export HADOOP_HOME=/home/vagrant/hadoop
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib
export GDAL_DATA=/usr/local/share/gdal
export GDAL_LIB_DIR=/usr/local/lib
export HOOT_WORKING_NAME=hoot
export PATH=$HOOT_HOME/bin:$PATH

# The actual run script
$CATALINA_HOME/bin/catalina.sh start
