#!/usr/bin/env bash

export JDK_VERSION=1.8.0_152
export JDK_URL=http://download.oracle.com/otn-pub/java/jdk/8u152-b16/aa0333dd3019491ca4f6ddbe78cdb6d0/jdk-8u152-linux-x64.tar.gz
export JDK_TAR=jdk-8u152-linux-x64.tar.gz
export JDK_MD5=20dddd28ced3179685a5f58d3fcbecd8

export GDAL_VERSION=2.1.4

# FGDB 1.5 is required to compile using g++ >= 5.1
# https://trac.osgeo.org/gdal/wiki/FileGDB#HowtodealwithGCC5.1C11ABIonLinux
export FGDB_VERSION=1.5.1
export FGDB_URL=https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_${FGDB_VERSION}/

# Hadoop versions and locations
export HADOOP_VERSION=0.20.2
export HADOOP_BASE_URL=https://archive.apache.org/dist/hadoop/core
export WITH_HADOOP="${WITH_HADOOP:-yes}"

# Ruby versions and locations.
export RUBY_BASE_URL=https://s3.amazonaws.com/hoot-ruby
export RUBY_GEMS_URL="${RUBY_BASE_URL}/gems"
export RUBY_VERSION=2.3.4

export RVM_BASE_URL=https://github.com/rvm/rvm
export RVM_BINARIES_URL="${RUBY_BASE_URL}/binaries"
export RVM_HOME="${RVM_HOME:-${HOME}/.rvm}"
export RVM_VERSION=1.29.3
export WITH_RVM="${WITH_RVM:-yes}"
